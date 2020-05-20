# PCO - Laboratoire 5 "Gestion de ressources"

Auteurs: Gabriel Roch, Cassandre Wojciechowski

## Description des fonctionnalités du logiciel

Dans le cadre de ce laboratoire, nous avons réalisé deux programmes.
Le premier permet à deux locomotives de partir depuis leur point de départ et de parcours un certain tracé. Nous avons nous-même défini ces tracés depuis le "main" du programme. Ces deux locomotives effectuent deux fois le parcours demandé, puis elles font demi-tour et cela d'une manière infinie. Sur le chemin, elles rencontrent des sections dites partagées : elles peuvent passer dessus toutes les deux. Nous avons donc mis en place un système de demande d'accès pour qu'une seule locomotive se trouve sur la section partagée à la fois. Quand une locomotive sort de cette section, l'autre peut y accéder à son tour. Si la section est occupée, la locomotive arrivée après doit s'arrêter et attendre.

Le second programme se comporte de manière similaire au premier mais les locomotives ont chacune un niveau de priorité différent. Cela change l'accès aux sections partagée car ce n'est plus la première arrivée qui passe d'abord mais celle qui a un niveau de priorité plus haut qui est favorisée. 


## Choix d'implémentation

Au niveau de notre implémentation, nous avons ajouté des classes supplémentaires à celles déjà présentes dans le projet, notamment pour gérer les parcours effectués par les locomotives. 

Dans la fonction LocomotiveBehavior::run(), nous avons implémenté le fonctionnement général d'une locomotive. La direction indiquée permet de savoir de quel côté du parcours nous commençons et dans quel sens nous allons l'effectuer. Au fur et à mesure du parcours, nous allons diriger les aiguillages pour aller aux endroits demandés et la locomotive va demander l'accès à chaque section partagée.
La locomotive avec la priorité la plus haute sera évidemment la première à y accéder, l'autre devant s'arrêter et attendre que la section soit libre.
Si la section partagée est déjà occupée, le thread de la locomotive effectuant l'appel sera suspendu grâce à la fonction "getAccess" et ne sera réveillé qu'au moment où l'autre locomotive sera effectivement sortie de la section. 
Après que le contact ait été fait (c'est-à-dire que la locomotive soit passée sur la section), nous faisons appel à la fonction "leave" qui va permettre au thread suspendu de la locomotive en attente de se réveiller.

Quand le parcours a été effectué deux fois, nous allons arrêter la locomotive, lui faire faire demi-tour et elle va repartir pour parcourir le même chemin mais dans le sens inverse.
Pour cela, nous avons créé une classe nommée "ParcoursIterator" afin d'éviter d'implémenter deux boucles "for" faisant presque exactement la même chose à l'exception du sens du parcours. 


## Tests effectués

Les testes suivants on tous été excécuté avec succés (il n'y a jamais eu de crash ou de bloquage définitif des loco) :

 - Mettre en pause une loco, puis la rédémarrer pour que les deux locos arrive en sens opposé sur la section partagée
 - Mettre en pause une loco, pour que la loco à faible priorité demande l'accés, mais se fait arrété car l'autre (haute priorité) à déjà fait un `request()`
 - refaire les tests ci-dessus avec et sans l'intertie d'actif.



