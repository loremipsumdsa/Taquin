#include "algo.h"

#define VIDE D*D // Valeur de la case 'vide' dans le tableau (carré de la dimension)

void afficher(int **t, int D){

	for (int i = 0; i<D; i++){  // Parcours du tableau
		
		for (int j = 0; j<D; j++)
			if (t[i][j] == VIDE)
				printf(" oo|");
			else
				printf(" %d |", t[i][j]);

		printf("\n");
	}
}

void liberer(int **t, int D){
	for (int i = 0; i < D; i++)
		free(t[i]);
	free(t);
}


int ** melanger(int D){

	// Creation du tableau
	int **t = 0;
	t = malloc(D * sizeof(int*));
	if (t == 0){  // Gestion d'erreur
		fprintf(stderr, "Erreur : Echec à la creation du tableau (niveau 1)\n");
		exit(1);
	}

	for (int i = 0; i<D; i++){
		t[i] = malloc(D * sizeof(int));
		if (t[i] == 0){ // Gestion d'erreur
			fprintf(stderr, "Erreur : Echec à la creation du tableau (niveau 2)\n");
			exit(1);
		}
	}
	// Remplissage ordonné du tableau
	int c = 1;
	for (int i = 0; i<D; i++)  // Parcours du tableau
		for (int j = 0; j<D; j++){

			t[i][j] = c; // On place chaque case à sa place
			c++;
	}

	// mélange par permutations aléatoires
	int n = 10000; //  nombre de tentatives de permutation
	char d; // deplacement ligne et colonne
	char direction[] = "bhdg";

    srand (time (NULL)); // initialisation du module alea

	for (int k = 0; k<n; k++){ // n fois

		// On fait en sorte d'avoir un déplacement aléatoire sur la ligne OU sur la colonne (pas les deux)
		d = direction[rand() % 4]; // On choisi une direction au hasard
		permuter(t, d, D); // On effectue la permutation si possible

	}

	sauvegarder(t, 0, D);
	return t;

}


void sauvegarder(int **t, int nbCoup, int D){
	FILE * f = NULL;
	f = fopen("sauv.txt", "w"); // Creation/ouverture du fichier
	if (f == NULL){ // Gestion d'erreur
		printf("Erreur : Echec creation de la sauvegarde\n");
		exit(1);
	}

	fprintf(f, "%d\n", nbCoup); //Sauvegarde du nombre de coup
	for (int i = 0; i<D; i++) // Parcours du tableau
		for (int j = 0; j < D; j++)
		{
			fprintf(f, "%d\n", t[i][j]); // écriture d'une case par ligne			
		}
	fclose(f); // Fermeture du fichier
}


int recharger(int **t, int D){
	char ligne[7];
	FILE * f = NULL;
	int nbCoup;

	f = fopen("sauv.txt", "r"); // Ouverture du fichier
	if (f == NULL){ // Gestion d'erreur
		printf("Erreur : Echec creation de la sauvegarde\n");
		exit(1);
	}

	fgets(ligne, 7, f); // Récuperation du nombre de coup
	sscanf(ligne, "%d\n", &nbCoup); // Extraction de la valeur	

	for (int i = 0; i<D; i++) // Parcours du tableau
		for (int j = 0; j < D; j++)
		{
			fgets(ligne, 7, f); // Récuperation de la ligne correspondante
			sscanf(ligne, "%d\n", &t[i][j]); // Extraction de la valeur			
		}
	fclose(f);

	return nbCoup; // On retourne le nombre de coup à l'instant de la sauvegarde

}


int permuter(int **t, char d, int D){

	for (int i = 0; i<D; i++)  // Parcours du tableau
		for (int j = 0; j<D; j++){
			
			if (t[i][j] == VIDE){ // On trouve la case à vide

				if (d == 'h' && i-1 >= 0){ // Si on va en haut
					t[i][j] = t[i-1][j]; // On permute
					t[i-1][j]  = VIDE;  
					return 1;

				}

				else if (d == 'b' && i+1 < D){ // Si on va en bas
					t[i][j] = t[i+1][j]; // On permute
					t[i+1][j]  = VIDE;  
					return 1;
					
				}

				else if (d == 'g' && j-1 >= 0){ // Si on va a gauche
					t[i][j] = t[i][j-1];  // On permute
					t[i][j-1] = VIDE;
					return 1;

				}

				else if (d == 'd' && j+1 < D){ // Si on va a droite
					t[i][j] = t[i][j+1];  // On permute
					t[i][j+1] = VIDE;
					return 1;
					
				}

				else{ // Si elle n'est pas dans le voisinage
					return 0; // On ne peut pas permuter
				}
			}
		}

	return 0;
}

int verifier(int **t, int D){
	
	int c = 1;

	for (int i = 0; i<D; i++)  // Parcours du tableau
		for (int j = 0; j<D; j++){

			if (t[i][j] == c) // On verifie si chaque case est bien à sa place
				c++;
			else
				return 0;  // Si une ne l'est pas, le tableau n'est pas solvé
		}
	return 1; // Sinon, il est solvé
}