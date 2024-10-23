# Igel Ärgern

## Un repo contenant le projet de système 1 : Igel Ärgern

Réalisé par : William et Simon

---

## Utilisation

./igel_argern <options>

Les options disponibles sont :
    
 - -h, -help : Aide sur les paramètres disponibles et sur l'utilisation du programme 
 - -lignes : Le nombre de lignes sur le plateau
 - -colonnes : Le nombre de colonnes sur le plateau
 - -joueurs : Le nombre de joueurs souhaités dans la partie
 - -herissons : Le nombre de hérissons par joueur
 - -objectif : Le nombre de hérissons nécéssaires pour gagner
 - -variante : Ajout d'une variante souhaitée (parmi "tube" ou "anarchie")


## Build

Nous avons utilisé l'utilitaire cmake afin de produire un makefile exhaustif relativement facilement. Afin d'utiliser ce fichier, il suffit de se placer dans le dossier build et de lancer cmake à cet endroit. L'utilisation du makefile produit engendrera un exécutable "igel_argern" à la racine du projet


## Contenu 
    - Le jeu de base
    - L'extension Tube
    - L'extension Anarchie
