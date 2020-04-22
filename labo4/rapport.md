
# Synchronisation de threads avec sémaphores

Auteurs: Gabriel Roch, Cassandre Wojciechowski

## Description des fonctionnalités du logiciel

Ce petit programme permet de gérer les montées et descentes de skieurs virtuels au moyen d'une télécabine virtuelle également. La télécabine effectue des trajets entre le bas et le sommet de la montagne. A la station du bas, des skieurs attendent de monter dans la télécabine. Quand elle arrive, un nombre N de skieurs montent puis la télécabine démarre sa montée. A l'arrivée au sommet, les skieurs sortent de la télécabine et descendent à skis. 

La télécabine descend en étant toujours vide, les skieurs ne descendent jamais dedans. 

A la fin du service de la télécabine, elle termine sa montée, décharge les skieurs et redescend. Les skieurs attendant de monter à la station doivent cesser d'attendre.  

## Choix d'implémentation

- Deux mutex pour la protection des variables `nbSkierWaiting` et `nbSkierInside`. Ces mutex sont implémenté à l'aide de sémaphore.

### Attente à la station basse

Lorsque des skieurs arrivent en-bas, ils demandent l'autorisation de monter dans une cabine avec un `acquire()` sur `semaphoreSkierGoIn`. Les threads des skieurs sont donc bloqués jusqu'au `release()` qui sera effectué par le thread de la cabine. Lorsque la cabine arrive en-bas et est prête à remonter, elle autorise le nombre de skieurs qu'elle peut accueillir à monter, en faisant des `release()` sur `semaphoreSkierGoIn`.



```sequence
participant A AS SkierA
participant B AS SkierB
participant C AS SkierC
participant S.GoIn AS GoIn
participant S.WaitGoUp AS GoUp
participant CableCar AS C
Note left of SkierA: 3 skieurs
Note left of SkierA:  en attente
SkierA -> GoIn: acquire()
SkierB -> GoIn: acquire()
SkierC -> GoIn: acquire()
Note right of C: START .loadSkiers()
C -> GoIn: 2 × release()
C -> GoUp: acquire()
GoIn --> SkierA: acquire() [OK]
Note right of SkierA: Go In
SkierA -> GoUp: release()
GoUp --> C: acquire() [OK]
C -> GoUp: acquire()
GoIn --> SkierB: acquire() [OK]
Note right of SkierB: Go In
SkierB -> GoUp: release()
GoUp --> C: acquire() [OK]
Note right of C: END .loadSkiers()
Note left of SkierA: 1 en attente
Note left of SkierA: 2 dans la cabine
```





## Tests effectués


Description de chaque test, et information sur le fait qu'il ait passé ou non

