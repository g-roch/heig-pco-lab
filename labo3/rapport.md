# PCO - Laboratoire 03

Cassandre Wojciechowski et Gabriel Roch

## Implémentation

### Découpe de la liste des mots de passe

Pour paralléliser le calcul des hashs, nous avons divisé la liste de mots de passe possibles selon le nombre de threads demandé par l'utilisateur.

Puisqu'il peut y avoir un reste à la division du nombre de mots de passe par le nombre de threads, le dernier thread effectue le hash de mot de passe supplémentaire. 

#### Exemple

Pour **3** threads et le charset **A-Z**, le premier thread effectue le premier tiers des mots de passe possibles (c'est à dire de AAA jusqu'à env. HHH). Le deuxième thread prendra de HHI jusqu'à env. PPP. Le dernier thread prendra de PPQ jusqu'à la fin y compris ZZZ.

### Gestion des threads

Chaque thread a besoin du mot de passe de départ et du nombre de mots de passe à hasher.

Pour le nombre de hashs, c'est juste la division du nombre de mots de passe possibles par le nombre de threads, sauf pour le dernier où l'on va jusqu'à la fin de la liste des possibilités.

Pour calculer le premier mot de passe d'un thread, on fait une addition sur le tableau représentant le password:

- On y additionne le nombre de calcul que doit faire le thread précédent
- L'addition se fait dans une base spéciale (qui correspond au nombre de caractères dans le charset)

### Affichage de la barre de progression

On met à jour la barre de progression tous les 1000 hashs et pour cela on utilise un Mutex pour éviter que 2 threads fassent l'opération en même temps. On a également besoin du nombre total de mots de passe possibles pour que l'incrément soit juste.

## Tests

| Password | Nb de threads | Temps [ms]     | Note                        | Réussi ? |
| -------- | ------------- | -------------- | --------------------------- | -------- |
| !*$      | 1, 3, 4       | 1633, 811, 898 |                             | Oui      |
| abc      | 1, 3, 5       | 49, 119, 197   |                             | Oui      |
| apz      | 1, 4, 7       | 604, 609, 651  |                             | Oui      |
| 095      | 1, 2, 6       | 1355, 640, 215 |                             | Oui      |
| zx       | 1, 2, 3       | 11, 13, 6      |                             | Oui      |
| apq!     | 2, 6          | 49902, 36531   |                             | Oui      |
| abcd     | 4             | 9813           |                             | Oui      |
| 99!      | 10            | 502            |                             | Oui      |
| p~73     | 16            | 17596          |                             | Oui      |
| qwe      | 3             | 77             | Salt=asdf                   | Oui      |
| !*$      | 2             | 525            | Salt=asdf                   | Oui      |
| ****     | 4             | 20854          | Dernier password du charset | Oui      |

**Note** : Les 3 derniers tests ont été faits sur une autre machine.

Au-delà de 4 caractères dans un mot de passe, le temps de cracking devient très long. Nous n'avons testé au maximum que 5 caractères "abcde" sur 7 threads et le temps de cracking était de 28.44 minutes. Pourtant les caractères de ce mot de passe se trouvent au début du charset. Cela démontre le temps de calcul nécessaire pour des mots de passe composés de beaucoup de caractères. 

Ici nous avons testé plusieurs sortes de cas différents, les mots de passe composés de 

- lettres
- chiffres
- caractères spéciaux
- un mélange des 3
- 2, 3, 4, 5 caractères
- caractères au début, milieu et fin du charset

D'après les tests récapitulés ci-dessus, nous pouvons constater que le programme fonctionne. En général, plus il y a de threads, plus le temps de recherche est court. Cependant au delà de 3 threads la progression est plus faible car nous n'avons que  2 threads de configurés sur la VM. Si les caractères composant le mot de passe se trouvent au début du charset (lettres minuscules), la recherche est bien plus rapide et dans ce cas, le nombre de threads importe peu et les temps sont relativement équivalents. 

Il faut aussi préciser que les temps de recherche dépendent des capacités de la machine utilisée, cela explique les variations observées entre certains tests. Il faut prendre en compte les ressources mobilisées pour d'autres applications qui pourraient tourner en même temps. 