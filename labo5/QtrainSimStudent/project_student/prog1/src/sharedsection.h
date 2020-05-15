//    ___  _________    ___  ___  ___  ___  //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  |/ _ \ //
//  / ___/ /__/ /_/ / / __// // / __// // / //
// /_/   \___/\____/ /____/\___/____/\___/  //
//                                          //
// Auteurs : Nom Prénom, Nom Prénom
//
#ifndef SHAREDSECTION_H
#define SHAREDSECTION_H

#include <QDebug>
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
     * Initialisez vos éventuels attributs ici, sémaphores etc.
     */
    SharedSection(std::string name) : name(name), accessMutex(1), free(true), waiting(1), nbLocoWaiting(0)
    {
        // TODO
    }

    /**
     * @brief request Méthode a appeler pour indiquer que la locomotive désire accéder à la
     * section partagée (deux contacts avant la section partagée).
     * @param loco La locomotive qui désire accéder
     * @param priority La priorité de la locomotive qui fait l'appel
     */
    void request(Locomotive& loco, Priority priority) override {
        // TODO

        // Exemple de message dans la console globale
        afficher_message(qPrintable(QString("The engine no. %1 requested the shared section.").arg(loco.numero())));
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
        // TODO
        bool stopped = false;
        accessMutex.acquire();

        while(not free and idLoco != loco.numero()){
            nbLocoWaiting++;
            accessMutex.release();

            if(not stopped){
                loco.arreter();
                stopped = true;
            }
            waiting.acquire();

            accessMutex.acquire();
            nbLocoWaiting--;
        }

        free = false;
        idLoco = loco.numero();
        ++count;
        accessMutex.release();
        //if(stopped){
        //    loco.demarrer();
        //}
            loco.demarrer();

        // Exemple de message dans la console globale
        afficher_message(qPrintable(QString(("The engine no. %1 accesses the shared section."+name).c_str()).arg(loco.numero())));
    }

    /**
     * @brief leave Méthode à appeler pour indiquer que la locomotive est sortie de la section
     * partagée. (reveille les threads des locomotives potentiellement en attente).
     * @param loco La locomotive qui quitte la section partagée
     */
    void leave(Locomotive& loco) override {
        // TODO
        afficher_message(qPrintable(QString(("Loco %1 leaves(), "+name).c_str()).arg(loco.numero())));
        accessMutex.acquire();
        --count;
        if(count == 0) {
          free = true;

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
    std::string name;
    PcoSemaphore accessMutex;
    bool free;
    PcoSemaphore waiting;
    unsigned int nbLocoWaiting;
    int idLoco;
    int count;
};


#endif // SHAREDSECTION_H
