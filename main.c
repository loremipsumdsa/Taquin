#include <stdio.h>
#include <stdlib.h> // Pour travailler avec des tableaux

#include "algo.h"
#include "graphique.h"

void console(int D){

	int **t = melanger(D); // Génération du tableau
	char d;
	printf("TAQUIN mode console by IZIGANG tm\n"); // Instructions
	printf("controles : \n");
	printf("- s : Sauvegarder\n");
	printf("- r : Recharger la sauvegarde\n");
	printf("h,b,d,g : Haut, Bas, Droite, Gauche\n");

	int nbCoup = 0; // compteur de coup

	while(!verifier(t, D)){ // Tant que le joueur n'a pas gagné
		printf("\n\n");
		afficher(t, D); // Affichage de la grille
		printf("\n");
		printf("Vers ou aller ? ( %d coup ) >",nbCoup); // Saisie utilisateur
		scanf("%c",&d);
		
		if (d=='s'){ // Si sauvegarde
			sauvegarder(t, nbCoup, D); // Sauvegarde
			printf("La grille à été sauvegardée !\n");
		}

		else if (d =='r'){ // Si restauration
			nbCoup = recharger(t,D); // Restauration
			printf("La grille à été rechargée !\n");
		}

		else { // Sinon (mouvement ou faute de frappe)
			
			if (!permuter(t,d, D)) // Tentative de permutation
				printf("permutation impossible !\n");
		nbCoup++; // Incrémentation du nombre de coup
		}

		scanf("%c",&d);
		printf("%c\n", d);
	}

	printf("\n\nBien joué ! \n"); // Si victoire, félicitation
	liberer(t, D); // Liberation de la mémoire

}

int main(int argc, char * argv[]){
	
	if (argc == 2)
		console(atoi(argv[1]));
	else
		graphique();
	return 0;
}