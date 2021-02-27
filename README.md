Bonjour !

Vous vous apprêtez à tester notre jeu de taquin. Veuillez préalablement lire les informations suivantes :

Prérequis : 

	Pour compiler le programme, vous aurez besoin d'avoir installé les librairies suivantes :
		
		- SDL 1.2. commande asociée* : libsdl1.2-dev
		- SDL TTF. commande associée* : sudo apt-get install libsdl-ttf2.0-dev
		- SDL Mixer. commande associée* : sudo apt-get install libsdl2-mixer-dev
	
	En plus des codes sources ( algo.c, algo.h, graphique.c, graphique.h, main.c) et du makefile, vous aurez besoins des dossiers contenants les dépendances multimédias du jeu : 
		- button
		- font
		- image
		- musics 
		- rect 
		- sound-effects

Compilation :
	- Ouvrez un terminal dans le dossier et tapez la commande make

Lancement du jeu :
	- Pour lancer le jeu en mode graphique : tapez la commande ./taquin
	- Pour lancer le jeu en mode console : tapez la commande ./taquin X   où X est la dimension de la grille voulu.


Toute l''équipe de développement IZIGANG tm vous souhaite d'avoir une superbe experience de jeu.



* Valable pour les distributions Ubuntu/Xubuntu
