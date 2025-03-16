# PROJET GRAPHES ET ALGO 

Le but de ce projet est de programmer un certain nombre d'algorithmes vus en cours et en
TD.
Il est question de créer un logiciel, avec une interface graphique, relativement complet
permettant de manipuler la notion de graphes.
Chaque groupe peut choisir l'interface et la bibliothèque graphiques qui lui conviennent
(TCL/TK, QT, MFC, OPEN GL (GLUT) etc…) de même que la plate-forme WINDOWS ou
UNIX (LINUX). Par contre le langage de programmation doit être obligatoirement orienté
objet (C++ ou Java) .
L'interface graphique doit être simple, clair et conviviale.
Le programme doit être capable de traiter un graphe orienté ou non orienté sous forme :
- de matrice d'adjacence.
- de file de successeurs (fs) et d'adresses du premier successeur (aps).
- de liste principale et de listes secondaires (totalement dynamique).
 Il doit également pouvoir passer d'une structure à l'autre.

1 - Livreur de classes :

Le chef de projet doit superviser la préparation est la mise en place des différentes classes
nécessaires à une programmation efficace de tous les algorithmes. Une analyse détaillée doit
être préparée (et présentée sous forme de mini rapport) pour l'élaboration de ces différentes
classes et ce par l'ensemble des membres du groupe.
Vous tiendrez compte du fait que le graphe peut représenter une situation où les sommets
ne sont pas réduits à un entier mais peuvent être des enregistrements complexes qui,
néanmoins, sont identifiés par des clés uniques allant de 1 à n (ordre du graphe).

2 - Les algorithmes :

a) La saisie : La saisie doit pouvoir se faire à partir :
- du clavier
- d'un fichier
- de la souris (graphiquement)
  
b) L'affichage : Le graphe doit pouvoir s'afficher :

- sur l'écran alphanumérique
- dans un fichier
- graphiquement
  
De manière optionnelle, le graphe affiché sous forme de points pour les sommets et de
segments de droite (ou de courbes) pour les arcs doit pouvoir être modifié interactivement
(ajout et suppression de sommets et d'arcs, déplacement de sommets, changement
d'orientation d'un arc, …).
Les parties saisie et affichage graphique peuvent être laissées en dernier (Le cœur de ce
projet reste la notion de graphe et il sera d'abord jugé sur le bon fonctionnement des différents
algorithmes).

c) Calcul des distances

d) détermination du rang des sommets

e) la détermination des composantes fortement connexes selon Tarjan
- Déterminez le graphe réduit.
- Éditez toutes les bases du graphe initial.
  
f) Les points d’articulation et les isthmes dans un graphe non orienté

g) le problème d'ordonnancement

Dans ce programme, l'utilisateur doit saisir les tâches du problème sous forme de
chaînes de caractères (voir exemple du cours). A chaque tâche, on affectera alors
automatiquement un numéro selon l'ordre d'entrée. Les tâches antérieures d'une tâche donnée
seront alors saisies sous forme du numéro correspondant. L'édition des chemins critiques sera
faite avec les noms des tâches correspondantes. Vous préparez un exemple concret pour tester
votre programme.

h) les chemins les plus courts selon
- Djikstra
- Dantzig
 Un contrôle doit être fait pour connaître les conditions d'application de chacun de ces
algorithmes.
Vous préparez un exemple autour d'une dizaine de villes françaises avec leurs distances
réelles pour illustrer l'algorithme de Dijkstra.

i) Détermination d'un arbre recouvrant minimal d'un graphe non orienté :
- Algorithme de Kruskal.

  Donnez un exemple concret pour illustrer le problème de l'arbre recouvrant minimal.

Dans l'application, vous devez avoir toujours un graphe courant, la possibilité de le
sauvegarder et de charger un autre graphe qui devient courant.
Une attention particulière sera portée sur le code.

Un travail d'investigation sera nécessaire pour bien choisir les exemples d’application..

Dans l'application, prévoyez un onglet qui permet d'expliquer brièvement chacun des
algorithmes étudiés en précisant la problématique qu'il traite, le principe de l'algorithme et les
conditions de son utilisation.. 

3 - Au rapport :

Le rapport du projet doit rendre compte des choix motivés des structures de données
utilisées. Il doit présenter brièvement les différents algorithmes, en précisant les structures
adoptées ainsi que leur complexité. Le fond et la forme du rapport seront jugés. 
