# Producteur-Consommateur pour calcul différé

Auteurs: Gabriel Roch, Cassandre Wojciechowski

### Etape 1

#### Choix d'implémentation

##### `int requestComputation(Computation c);`

Nous avons créé un vector de taille 3 contenant des "Request" nommé `queueInput` pour que chaque cellule du vector corresponde à un type de calcul. Nous avons également créé un vector `inputNotFullNbThreadWaiting` comme celui décrit précédemment mais contenant des "int" afin de compter combien de calculs de chaque type sont à faire.

Nous attribuons directement un identifiant au calcul.

Pour que la requête de calcul soit mise en attente, il faut que l'arrêt des threads n'ait pas été demandé et que plus de `MAX_TOLERATED_QUEUE_SIZE` calculs de ce type aient été demandés. Si la requête passe les conditions ci-dessus, alors nous allons incrémenter le nombre de requête de son type en attente dans le vector et effectuer le `wait()`. 

Si l'arrêt des threads est demandé, alors on sort du moniteur et on lance l'exception dédiée `throwStopException`.

Nous mettons effectivement la requête dans le vector `queueInput`, puis nous allons ensuite faire le signal pour réveiller un autre thread qui pourra lancer une demande de calcul à son tour. La méthode retourne l'identifiant du calcul qui vient d'être demandé. 

##### `Request getWork(ComputationType computationType);`

Nous avons créé un vector `inputNotEmptyNbThreadWaiting` comme ceux décrits précédemment contenant des `int `  afin de compter combien de calculs de chaque type sont à faire. 

Nous testons si l'arrêt des threads a été demandé et si aucune requête pour un calcul du type passé en paramètre n'est présente dans `queueInput`. Si ces conditions sont remplies, alors nous incrémentons le nombre de calculs de ce type qui sont attendus par la fonction `getWork` et nous faisons un `wait()` afin d'endormir le thread tant qu'il n'y a rien a faire. 

Si l'arrêt des threads est demandé, alors on sort du moniteur et on lance l'exception dédiée `throwStopException`.

Nous allons ensuite récupérer la requête qui a été mise dans le vector `queueInput`, la stocker dans une variable Request `front` et la supprimer du vector. Cela permet de garder l'ordre des requêtes. Nous effectuons un `signal()` pour qu'un thread se réveille et puisse effectuer une nouvelle requête et on retourne la requête `front`. 

#### Tests

- Faire 15 demandes de calcul pour 1 type et vérifier si l'attente est correctement faite : 
  - Les calculs se font dans l'ordre correct et les résultats apparaissent les uns après les autres
- Tester d'appuyer sur stop 
  - Quand on appuie sur le bouton stop, tous les calculs sont arrêtés

### Etape 2

#### Choix d'implémentation

##### `Result getNextResult());`

Nous avons créé une queue de priorité `queueOutput` qui va permettre de gérer la priorité des résultats à afficher ainsi qu'une seconde queue de priorité `nextAbortResult` pour gérer le cas où un calcul aurait été annulé après avoir été effectué et avant d'avoir été affiché. 

Tant que l'arrêt des threads n'est pas demandé et qu'il n'a a rien dans la queue des résultats à afficher ou que l'identifiant du prochain calcul à afficher n'est pas le même que celui qu'on devrait afficher, alors on va tester si le prochain résultat à afficher a été annulé. Si c'est le cas, on incrémente l'identifiant du prochain résultat à afficher. On effectue ensuite un `wait()` sur la condition `outputNotEmpty` pour endormir le thread si le prochain résultat n'est pas encore disponible.

Comme dans les méthodes précédentes, si les threads doivent s'arrêter alors on sort du moniteur et on lance l'exception. 

Ensuite, nous incrémentons l'identifiant du prochain résultat à afficher à l'écran et nous récupérons celui que nous voulons afficher actuellement depuis la queue dans une variable `ret`. On retire ce résultat de la queue et on le retourne à la fin de la méthode. 

##### `void provideResult(Result result);`

La méthode provideResult permet d'afficher à l'écran le résultat et de le mettre dans la queue de priorité `queueOutput`. 

Elle va ensuite faire un signal sur la condition `outputNotEmpty` pour réveiller des threads qui voudraient mettre un résultat dans la queue dans la méthode `getNextResult`. 

#### Tests

Vérifier que les calculs mis en attente se font ensuite dans l'ordre après avoir été réveillés

- Quand le 10ème calcul a été effectué, les suivants se font dans l'ordre croissant des identifiants

### Etape 3

#### Choix d'implémentation

##### `void abortComputation(int id);`

Nous avons créé un set `abortedSet` permettant de stocker les identifiants des calculs qui ont été annulés. 

Dans la méthode `abortComputation`, nous allons insérer les identifiants des calculs annulés (passés en paramètre de la méthode) dans `abortedSet` et également dans la queue de priorité `nextAbortResult` (pour gérer l'affichage des résultats).

Nous faisons ensuite un signal sur la condition `outputNotEmpty` pour réveiller des threads qui voudraient obtenir un résultat dans la queue dans la méthode `getNextResult` pour qu'il soit en mesure d'ignorer ceux qui sont annulé.

##### `bool continueWork(int id);`

Cette méthode va chercher l'identifiant du calcul (passé en paramètre) dans `abortedSet`. Si elle le trouve, elle va le supprimer du set et retourner `false` pour indiquer qu'il ne faut pas continuer à travailler sur ce calcul car il a été annulé. 

Si elle ne le trouve pas, alors elle va retourner `true and not stopped` car il faut de nouveau vérifier si les threads n'ont pas été arrêtés entre temps.

#### Tests

Vérifier que l'annulation d'un calcul l'empêche bien d'arriver au bout

- En annulant un calcul, on constate qu'immédiatement, la barre rouge apparaît dans la ligne des calculs en exécution

Vérifier qu'un calcul annulé n'est pas affiché

- En annulant un calcul, la barre grise apparaît directement dans la ligne des résultats avec l'identifiant du calcul et on ne voit pas de barre verte annonçant un résultat avec ce même identifiant

### Etape 4

#### Choix d'implémentation

##### `void stop();`

Dans cette méthode, on va setter la variable booléenne `stopped` à vrai et réveiller les threads en attente. Pour réveiller les threads, nous utilisons trois boucles `for` pour être sûr de réveiller tous les threads sur lesquels nous avons pu faire un `wait()`. 

##### `private: void throwStopException();`

Cette méthode va permettre d'afficher que l'exception a été lancée et va ensuite lancer l'exception `StopException()`. 

#### Tests

Vérifier que l'exception est lancée correctement 

- On peut lire dans l' `Application Output`  que l'exception a bien été lancée

Vérifier qu'en appyuant sur `Stop`tous les threads s'arrêtent

- Les threads sont arrêtés et rejoignent le thread principal

