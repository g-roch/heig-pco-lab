#include <QCryptographicHash>
#include <QVector>

#include "threadmanager.h"
#include <pcosynchro/pcothread.h>
#include "mythread.h"

/*
 * std::pow pour les long long unsigned int
 *
 * reds@reds2019:~$ echo -n "zzz" | md5sum
f3abb86bd34cf4d52698f14c0da1dc60  -
reds@reds2019:~$

Sur 3 thread
 */
long long unsigned int intPow (
        long long unsigned int number,
        long long unsigned int index)
{
    long long unsigned int i;

    if (index == 0)
        return 1;

    long long unsigned int num = number;

    for (i = 1; i < index; i++)
        number *= num;

    return number;
}

ThreadManager::ThreadManager(QObject *parent) :
    QObject(parent)
{}


void ThreadManager::incrementPercentComputed(double percentComputed)
{
    emit sig_incrementPercentComputed(percentComputed);
}

/*
 * Les paramètres sont les suivants:
 *
 * - charset:   QString contenant tous les caractères possibles du mot de passe
 * - salt:      le sel à concaténer au début du mot de passe avant de le hasher
 * - hash:      le hash dont on doit retrouver la préimage
 * - nbChars:   le nombre de caractères du mot de passe à bruteforcer
 * - nbThreads: le nombre de threads à lancer
 *
 * Cette fonction doit retourner le mot de passe correspondant au hash, ou une
 * chaine vide si non trouvé.
 */
QString ThreadManager::startHacking(
        QString charset,
        QString salt,
        QString hash,
        unsigned int nbChars,
        unsigned int nbThreads
)
{

    long long unsigned int nbToCompute;
    long long unsigned int nbToComputeSended;
    /*
     * Nombre de caractères différents pouvant composer le mot de passe
     */
    unsigned int nbValidChars;

    nbToCompute        = intPow(charset.length(),nbChars);
    nbToComputeSended  = 0;
    /*
     * Nombre de caractères différents pouvant composer le mot de passe
     */
    nbValidChars       = charset.length();

    /*
     * Tableau contenant les index dans la chaine charset des caractères de
     * currentPasswordString
     */
    QVector<unsigned int> currentPasswordArray;

    currentPasswordArray.fill(0,nbChars);

    ended = false;
    passowrdFound = "";

    QVector<PcoThread*> threadVector;

    MyThread myThread(*this, charset, salt, hash, nbToCompute);

    // Pour chaque thread à créer
    for(unsigned int i = 0; i < nbThreads; ++i) {
        long long unsigned int toCalc = nbToCompute / nbThreads;
        if(i == nbThreads -1) {
            toCalc = nbToCompute - nbToComputeSended;
        }
        nbToComputeSended += toCalc;
        PcoThread * t = new PcoThread(&MyThread::start, &myThread, currentPasswordArray, toCalc);

        // Incrément le pwd
        unsigned int j = 0;
        long long unsigned int tmp = toCalc;
        while (j < (unsigned int)currentPasswordArray.size()) {
            currentPasswordArray[j] += tmp % nbValidChars;
            //tmp -= tmp % nbValidChars;
            tmp /= nbValidChars;

            if (currentPasswordArray[j] >= nbValidChars) {
                currentPasswordArray[j] -= nbValidChars;
                ++tmp;
            } else if(tmp == 0)
                break;
            ++j;
        }

        threadVector.push_back(t);
    }
    for(unsigned int i = 0; i < nbThreads; ++i) {
        threadVector.at(i)->join();
        delete threadVector.at(i);
    }

    return myThread.getPassword();
}

