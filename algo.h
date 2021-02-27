#include <stdio.h>
#include <stdlib.h> // Pour travailler avec des tableaux
#include <time.h> // Pour generer des nombres aléatoires

int ** melanger(int D); // Retourne une configuration solvable de taille D
void sauvegarder(int **t, int nbCoup, int D); // Sauvegarde la configuration t de taille D dans un fichier
int recharger(int **t, int D);  // Importe une configuration de taille D depuis un fichier dans t
int permuter(int **t, char d, int D); // effectue si possible la permutation n <-> VIDE, retourne 1 si possible, 0 sinon
int verifier(int **t, int D); // Retourne 1 si le tableau est arrangé, 0 sinon

void afficher(int **t, int D);
void liberer(int **t, int D);