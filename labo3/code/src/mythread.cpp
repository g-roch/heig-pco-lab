#include "mythread.h"
#include "threadmanager.h"
#include <QCryptographicHash>
#include <QVector>

MyThread::MyThread(
        ThreadManager & threadManager,
        QString const & charset,
        QString const & salt,
        QString const & hash,
        long long unsigned nbTotalHash
        ) :
    passowrdFound(),
    ended(false),
    mutexProgress(),
    threadManager(threadManager),
    charset(charset),
    salt(salt),
    hash(hash),
    nbTotalHash(nbTotalHash)
{
}


QString MyThread::getPassword() const
{
    return passowrdFound;
}

void MyThread::start(
        QVector<unsigned int> currentPasswordArray,
        long long unsigned int nbToCompute
        )
{

    unsigned int i;

    long long unsigned int nbComputed;
    long long unsigned int progressed = 0;

    /*
     * Nombre de caractères différents pouvant composer le mot de passe
     */
    unsigned int nbValidChars;

    /*
     * Mot de passe à tester courant
     */
    QString currentPasswordString;


    /*
     * Hash du mot de passe à tester courant
     */
    QString currentHash;

    /*
     * Object QCryptographicHash servant à générer des md5
     */
    QCryptographicHash md5(QCryptographicHash::Md5);

    /*
     * Calcul du nombre de hash à générer
     */
    nbComputed         = 0;

    /*
     * Nombre de caractères différents pouvant composer le mot de passe
     */
    nbValidChars       = charset.length();

    /*
     * On initialise le premier mot de passe à tester courant en le remplissant
     * de nbChars fois du premier caractère de charset
     */
    currentPasswordString.fill(charset.at(0),currentPasswordArray.size());

    /*
     * Tant qu'on a pas tout essayé...
     */
    while (nbComputed < nbToCompute and !ended) {

        /*
         * On traduit les index présents dans currentPasswordArray en
         * caractères
         */
        for (i=0;i<currentPasswordArray.size();i++)
            currentPasswordString[i]  = charset.at(currentPasswordArray.at(i));

        /* On vide les données déjà ajoutées au générateur */
        md5.reset();
        /* On préfixe le mot de passe avec le sel */
        md5.addData(salt.toLatin1());
        md5.addData(currentPasswordString.toLatin1());
        /* On calcul le hash */
        currentHash = md5.result().toHex();

        /*
         * Si on a trouvé, on retourne le mot de passe courant (sans le sel)
         */
        if (currentHash == hash) {
            passowrdFound = currentPasswordString;
            ended = true;
            return;
        }

        /*
         * Tous les 1000 hash calculés, on notifie qui veut bien entendre
         * de l'état de notre avancement (pour la barre de progression)
         */
        if ((nbComputed % 1000) == 0) {
            mutexProgress.lock();
            threadManager.incrementPercentComputed((double)1000/nbTotalHash);
            mutexProgress.unlock();
            progressed = nbToCompute;
            //incrementPercentComputed((double)1000/nbToCompute);
        }//*/

        /*
         * On récupère le mot de pass à tester suivant.
         *
         * L'opération se résume à incrémenter currentPasswordArray comme si
         * chaque élément de ce vecteur représentait un digit d'un nombre en
         * base nbValidChars.
         *
         * Le digit de poids faible étant en position 0
         */
        i = 0;

        while (i < (unsigned int)currentPasswordArray.size()) {
            currentPasswordArray[i]++;

            if (currentPasswordArray[i] >= nbValidChars) {
                currentPasswordArray[i] = 0;
                i++;
            } else
                break;
        }


        nbComputed++;
    }
/*
    mutexProgress.lock();
    //incrementPercentComputed((double)(nbToCompute - progressed)/nbToCompute);
    mutexProgress.unlock();
//*/
    /*
     * Si on arrive ici, cela signifie que tous les mot de passe possibles ont
     * été testés, et qu'aucun n'est la préimage de ce hash.
     */
    return;
}
