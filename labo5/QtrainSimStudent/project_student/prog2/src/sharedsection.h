//    ___  _________    ___  ___  ___  ___  //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  |/ _ \ //
//  / ___/ /__/ /_/ / / __// // / __// // / //
// /_/   \___/\____/ /____/\___/____/\___/  //
//                                          //
// Auteurs : Roch Gabriel, Wojciechowski Cassandre
//
#ifndef SHAREDSECTION_H
#define SHAREDSECTION_H

#include <QDebug>
#include <map>
#include <string>

#include <pcosynchro/pcosemaphore.h>

#include "locomotive.h"
#include "ctrain_handler.h"
#include "sharedsectioninterface.h"

/**
 * @brief La classe SharedSection implémente l'interface SharedSectionInterface qui
 * propose les méthodes liées à la section partagée.
 */
class SharedSection final : public SharedSectionInterface
{
  public:

    /**
     * @brief SharedSection Constructeur de la classe qui représente la section partagée.
     */
    SharedSection(std::string name) : name(name), accessMutex(1), free(true), waiting(1), nbLocoWaiting(0), prioritiesCount() { }

    /**
     * @brief request Méthode a appeler pour indiquer que la locomotive désire accéder à la
     * section partagée (deux contacts avant la section partagée).
     * @param loco La locomotive qui désire accéder
     * @param priority La priorité de la locomotive qui fait l'appel
     */
    void request(Locomotive& loco, Priority priority) override {
      // Protection de l'accès à la variable avec un mutex
      accessMutex.acquire();
      // Incrémentation du nb de loco avec la priorité priority effectuant une requête sur une section
      ++prioritiesCount[priority];
      accessMutex.release();

      // Exemple de message dans la console globale
      afficher_message(qPrintable(QString(("The engine no. %1 requested the shared section."+name).c_str()).arg(loco.numero())));
    }

    /**
     * @brief getAccess Méthode à appeler pour accéder à la section partagée, doit arrêter la
     * locomotive et mettre son thread en attente si la section est occupée ou va être occupée
     * par une locomotive de plus haute priorité. Si la locomotive et son thread ont été mis en
     * attente, le thread doit être reveillé lorsque la section partagée est à nouveau libre et
     * la locomotive redémarée. (méthode à appeler un contact avant la section partagée).
     * @param loco La locomotive qui essaie accéder à la section partagée
     * @param priority La priorité de la locomotive qui fait l'appel
     */
    void getAccess(Locomotive &loco, Priority priority) override {
        afficher_message(qPrintable(QString(("The engine no. %1 getAccess()."+name).c_str()).arg(loco.numero())));

        bool stopped = false;
        accessMutex.acquire();

        // Tant que la loco ne peut pas accéder à la section, on incrémente le nb de loco en attente et elle attend
        while(not canAccess(loco, priority)){
            nbLocoWaiting++;
            accessMutex.release();

            // On fait en sorte que la loco s'arrête si elle ne l'est pas déjà
            if(not stopped){
                loco.arreter();
                stopped = true;
            }

            // Accès à la section
            waiting.acquire();

            accessMutex.acquire();
            nbLocoWaiting--;
        }

        // La section n'est pas libre
        free = false;
        // On récupère l'id de la loco
        idLoco = loco.numero();
        // Incrémente le nombre de loco sur la section
        ++count;
        // On décrémente le nb de loco avec la priorité priority en train d'attendre
        --prioritiesCount[priority];
        accessMutex.release();

        loco.demarrer();

        // Exemple de message dans la console globale
        afficher_message(qPrintable(QString(("The engine no. %1 accesses the shared section."+name).c_str()).arg(loco.numero())));
    }

private:
    /**
     * @brief canAccess Teste si la locomotive peut accéder à la section ou si une autre
     * locomotive a une priorité plus élevée
     * @param loco La locomotive qui veut accéder à la section partagée
     * @param priority La priorité de la locomotive loco
     */
    bool canAccess(Locomotive &loco, Priority priority) const {
      bool ret = free;
      for(auto p : prioritiesCount) {
        // On vérifie si il y a d'autres locomotives avec une priorité plus élevée
        if(p.first > priority && p.second) {
          // Si oui, on ne peut pas accéder
          ret = false;
        }
      }
      return ret or idLoco == loco.numero();
    }

public:
    /**
     * @brief leave Méthode à appeler pour indiquer que la locomotive est sortie de la section
     * partagée. (reveille les threads des locomotives potentiellement en attente).
     * @param loco La locomotive qui quitte la section partagée
     */
    void leave(Locomotive& loco) override {
        afficher_message(qPrintable(QString(("Loco %1 leaves(), "+name).c_str()).arg(loco.numero())));

        accessMutex.acquire();
        // On décrémente le nombre de loco sur la section
        --count;

        // S'il n'y en a plus sur la section, alors elle est libre
        if(count == 0) {
          free = true;

          // On relâche s'il y a des locomotives en attente
          if(nbLocoWaiting > 0){
            waiting.release();

            afficher_message(qPrintable(QString("Loco %1 release()").arg(loco.numero())));
          }
        }

        accessMutex.release();

        // Exemple de message dans la console globale
        afficher_message(qPrintable(QString(("The engine no. %1 leaves the shared section."+name).c_str()).arg(loco.numero())));
    }

private:
    // Nom de la section
    std::string name;
    // Sémaphore utilisé comme mutex pour protéger l'accès aux variables
    PcoSemaphore accessMutex;
    // Indique si la section est libre
    bool free;
    // Sémaphore pour accéder à la section
    PcoSemaphore waiting;
    // Nombre de loco en attente
    unsigned int nbLocoWaiting;
    // Stocke le nombre de loco d'une priorité p dans une map
    std::map<Priority, int> prioritiesCount;
    // Récupère le numéro de la loco
    int idLoco;
    // Indique le nombre de loco sur une section
    int count;
};


#endif // SHAREDSECTION_H
