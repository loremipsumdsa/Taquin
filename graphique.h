#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <SDL/SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "algo.h"

void graphique();

int randomMusic(void * musique);

int menu();

SDL_Surface * initialisation(int n,int width, int height);//Renvoie un pointeur vers une surface video de largeur width et de hauteur height avec pour image de fond l'image numéro i..

SDL_Surface** createRect(int n,char debutNom[20]);//Renvoie un pointeur sur n pointeurs qui pointent chacun sur une image. Le deuxième paramètre est le début du chemin vers les images

void setButtonMenu(SDL_Surface*screen,SDL_Surface**button);//Affiche sur l'écran de menu les images des boutons Nouvelle partie, Recharger et Quitter.

int eventMenu(SDL_Surface*screen,SDL_Surface**button);// Permet à l'utilisateur de communiquer avec le menu à partir de ses entrées souris. Renvoie 1 si l'utilisateur décide d'arrêter totalement le jeu, renvoie -1 autrement.

void colorButton(SDL_Surface*screen,SDL_Surface**button,int num,int x, int y);//Affiche l'image du bouton de la case "num" de "button" à l'emplacement x,y.

int clickOnMenu(Mix_Chunk*sonClic,int x,int y);//Permet de gérer un clic à l'emplacement [x;y] sur le menu. Renvoie 1 si l'utilisateur a décidé d'arrêter totalement le jeu, -1 si l'utilisateur a cliqué sur un bouton pour lancer une partie et 0 s'il a cliqué sur un élément neutre.

void jeu(int**tab,int nbCoups);//Lance le jeu de taquin avec une grille tab et un nombre de coups nbCoups.

void synchro(SDL_Surface*screen,SDL_Surface**rect,int**tab,int nbCoups);//Affiche les images des cases de la grille synchronisées aux cases de la grille dans tab ainsi que le nombre de coups. 

void setButtonJeu(SDL_Surface*screen,SDL_Surface**button,int **tab);//Affiche sur l'écran de jeu les images des boutons Sauvegarder, Recharger et Quitter.

void eventJeu(SDL_Surface*screen,SDL_Surface**rect,SDL_Surface**button,int**tab, int nbCoups);//Permet à l'utilisateur de communiquer avec le jeu à partir de ses entrées clavier et souris.

void achievement(SDL_Surface*screen);//Affiche l'écran de réussite du jeu de taquin sur l'écran de jeu.

int clickOnJeu(SDL_Surface*screen,SDL_Surface**rect,SDL_Surface**button,int**tab,int*nbCoups,Mix_Chunk*sonClic,int x,int y);//Permet de gérer un clic à l'emplacement [x;y] sur le jeu. Renvoie 1 si l'utilisateur décide de quitter le jeu de taquin et 0 s'il a cliqué sur un autre élément.

void freeSurface(SDL_Surface**rect, int n);//Libère la mémoire pointée par un pointeur sur un pointeur de surface.

extern int done;
