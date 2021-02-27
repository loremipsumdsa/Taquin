#include "graphique.h"

int done=0;//Variable globale permettant à la fonction qui tourne dans le thread secondaire de savoir à tout moment si le thread principale doit se terminer.

void graphique(){

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)==-1){ //Si l'initialisation des modules audio et video échoue
    fprintf(stderr,"Unable to initialize VIDEO or AUDIO:%s \n",SDL_GetError()); //On écrit un message d'erreur
    exit(EXIT_FAILURE); //On sort du programme
  }

  if (TTF_Init()==-1){ //Si l'initialisation de SDL_Ttf échoue
    fprintf(stderr,"Unable to initialize TTF: %s\n",TTF_GetError()); //On écrit un message d'erreur
    exit(EXIT_FAILURE); //On sort du programme
  }

  if (Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,256)==-1){//Si l'initialisation de SDL_Mixer échoue
    fprintf(stderr,"Unable to open an audio with Mixer: %s",Mix_GetError());//On écrit un message d'erreur
    exit(EXIT_FAILURE);//On sort du programme
  }

  srand(time(NULL)); //On enclenche le module alea

  Mix_Music * musique=NULL;//On crée un pointeur vers une musique
  
  SDL_Thread* threadMusic = SDL_CreateThread(randomMusic,musique);//On lance un thread qui va jouer des musiques aléatoires en fond

  int stop=0; //On initialise la fonction qui va décider de si le jeu s'arrête ou pas
  while(stop==0||stop==-1){//Si stop est à 0 ou -1 le jeu continue et on relance menu()
    stop=menu(); //La fonction menu renvoi soi 0, soi -1, soi 1
  }

  done = 1;//Le jeu est terminé donc on met la variable globale done à 1 pour stopper le thread
  
  SDL_WaitThread(threadMusic,NULL); //On attend que le thread se stop grâce à la variable done
  
  Mix_FreeMusic(musique);//La musique n'est plus utilisée donc on libère la mémoire correspondante
  
  Mix_Quit(); //On quitte SDL_Mixer
  TTF_Quit(); //On quitte SDL_Ttf
  SDL_Quit(); //On quitte SDL_Quit
}

int randomMusic(void * musique){
  Mix_Music * newMusique = (Mix_Music*) musique;//On cast musique pour pouvoir l'utiliser en tant que pointeur vers une musique
  Mix_VolumeMusic(65); //Le volume de base est à 128, ce qui était trop fort donc on le met à 65
  while (done==0){ //Tant que done==0 donc tant que le main thread tourne donc tant que le jeu n'est pas terminé
    SDL_Delay(1000); //On delay pour éviter de faire souffrir le CPU avec une boucle qui se répète trop vite pour rien. La boucle va donc se répéter toutes les secondes
    if (Mix_PlayingMusic()==0){//Si aucune musique ne joue actuellement
      char nomFichier[14];//On crée une chaine de char qui va contenir le chemin d'une nouvelle musique
      sprintf(nomFichier,"music/%i.mp3",rand()%54); //On met dans cette chaine le chemin d'une nouvelle musique aléatoire, il y en a 53 donc on utilise %54
      Mix_FreeMusic(newMusique); //On libère la mémoire allouée par une potentielle musique pointée par newMusique
      newMusique=NULL;//On set le pointeur à NULL, si l'allocation de la ligne suivante ne s'effectue pas ne pointeur ne pointera pas n'importe où
      newMusique= Mix_LoadMUS(nomFichier);//On alloue de la mémoire, on y met la musique aléatoire et on pointe cette case avec newMusique
      Mix_PlayMusic(newMusique,0); //On lance la musique, elle ne se répète pas
    }
  }
  return 0;
}

int menu(){

  SDL_Surface * screen=initialisation(0,480,480); //On appelle initialisation() pour créer la fenêtre et la pointer avec screen

  SDL_Surface ** button = createRect(8,"button/button"); //On utilise createRect() pour créer une une liste de 8 surfaces correspondant aux boutons "nouvelle partie", "recharger" et "quitter" qui s'appellera button. (8 car il y a les images grises et bleus)
  
  setButtonMenu(screen,button); // On utilise cette fonction pour afficher les boutons sur l'écran screen

  int stop=eventMenu(screen,button);//Dans stop on va stocker la décision du joueur quant au fait d'arrêter le jeu totalement (1) ou de juste relancer le menu pour arrêter le taquin(0)

  SDL_FreeSurface(screen);//Puisqu'on va redémarrer le menu ou arrêter le jeu, on ferme l'écran donc on libère la mémoire allouée par screen pour l'écran

  freeSurface(button,8);//On libère la mémoire allouée pour tous les boutons dans la liste button.
  
  return stop;//On renvoie 1 pour arrêter le jeu totalement, 0 pour redémarrer le menu et juste arrêter le taquin
}

SDL_Surface * initialisation(int n,int width,int height){
  
  SDL_Surface * screen=NULL; //screen va contenir la fenêtre
  if ((screen= SDL_SetVideoMode(width,height,32,SDL_HWSURFACE))==NULL){
    fprintf(stderr,"Ok erreur sdl:%s \n",SDL_GetError());
    exit(EXIT_FAILURE);
  } //Si on ne parvient pas à créer la fenêtre on stop le prog et un message d'erreur s'envoie

  SDL_Rect pos; //Variable contenant la position de l'image de fond de la fenêtre
  pos.x=0;//coord axe horizontal
  pos.y=0;//coord axe vertical

  char nomFichier[22]; //chaine de char contenant le chemin vers l'image de fond
  sprintf(nomFichier,"image/background%i.bmp",n); //Ecriture du chemin dans la chaine
  SDL_Surface *background=SDL_LoadBMP(nomFichier);//Creation de la surface pointant vers l'image de fond

  SDL_BlitSurface(background,NULL,screen,&pos); //Posage de l'image de fond au coord [0;0]

  SDL_FreeSurface(background);//On libère la mémoire allouée pour l'image de fond puisqu'on l'a déjà posé
  
  return screen;//On renvoi le pointeur vers la fenêtre qui contient désormais un fond
}

SDL_Surface** createRect(int n,char debutNom[20]){

  SDL_Surface ** rect=(SDL_Surface**) malloc(n*sizeof(SDL_Surface*)); //On crée une liste de n surfaces

  char nomFichier[27]; //Chaine de char contenant le chemin du type d'image que va contenir la liste rect
  
  for (int i=0;i<n;i++){//Pour le nombre de surface
    sprintf(nomFichier,"%s%i.bmp",debutNom,i);//On met dans la chaine le chemin du type d'image que va contenir la liste rect
    rect[i]=SDL_LoadBMP(nomFichier);//On fait pointer chaque surface sur leur image correspondante
  }
   
  return rect;//On renvoi la liste
}

void setButtonMenu(SDL_Surface*screen,SDL_Surface**button){

  SDL_Rect pos; //Variable contenant la position des boutons que l'on va placer
  pos.x=80; //coord axe horizontal
  pos.y=80; //coord axe vertical
  //Position du bouton Nouvelle partie

  SDL_BlitSurface(button[0],NULL,screen,&pos); //Posage du bouton Nouvelle partie aux dernières coordonnées de pos sur la fenêtre

  pos.y=pos.y+125; //Position du bouton Recharger

  SDL_BlitSurface(button[1],NULL,screen,&pos);//Posage du bouton Recharger aux dernières coordonnées de pos sur la fenêtre

  pos.y=pos.y+175; //Position du bouton Quitter
  
  SDL_BlitSurface(button[2],NULL,screen,&pos); //Posage du bouton Quitter aux dernières coordonnées de pos sur la fenêtre

  SDL_Flip(screen);//On met en place les modifications pour obtenir une belle fenêtre de menu

}

int eventMenu(SDL_Surface*screen,SDL_Surface**button){
  int stop=0; //Variable à 1 si on arrête totalement le jeu, à -1 si on relance le menu
  int x,y;//Variables qui vont comprendre la position de la souris quand elle bouge
  Mix_Chunk * sonClic= NULL;//Pointeur qui va pointer sur un effet sonore de clic
  sonClic = Mix_LoadWAV("sound_effect/1.wav");//On le fait pointer vers l'effet sonore
  SDL_Event event; //Variable contenant l'input du joueur
  while(stop==0){//Tant qu'on ne décide pas d'arrête ni de relancer le menu
    SDL_WaitEvent(&event); //On attend que le joueur interagisse avec le jeu
    switch(event.type){ //On étudie tous les cas d'input
    case SDL_QUIT: //Si il ferme d'une quelconque façon la fenêtre le jeu s'arrête totalement
      stop=1;//le jeu s'arrête totalement
      break;
    case SDL_MOUSEBUTTONDOWN: //Si il clique à un endroit
      if (event.button.button==SDL_BUTTON_LEFT){//avec le bouton gauche de la souris
	stop=clickOnMenu(sonClic,event.button.x,event.button.y);//On arrête le programme en fonction de l'endroit du clic
      }
      break;
    case SDL_MOUSEMOTION://Si il bouge la souris
      x=event.motion.x;//x contient sa position horizontal
      y=event.motion.y;//y contient sa position vertical
      if (x>=80 && x<=400 && y>=80 && y<=144){//si il passe sur le bouton Nouvelle partie
	colorButton(screen,button,4,80,80);//On met le bouton nouvelle partie en bleu
      }

      else if (x>=80 && x<=400 && y>=205 && y<=269){//Si il passe sur le bouton Recharger
	colorButton(screen,button,5,80,205);//On met le bouton Recharger en bleu
      }

      else if (x>=80 && x<=400 && y>=380 && y<=444){ //Si il passe sur le bouton Quitter
	colorButton(screen,button,6,80,380);// On met le bouton Quitter en bleu
      }
      else { //S'il ne passe sur aucun bouton
	setButtonMenu(screen,button);//On réaffiche tous les boutons au cas où certains seraient affichés en bleu.
      }
      break;

    default:
    break;
    }
    }
  Mix_FreeChunk(sonClic);//On libère la mémoire allouée pour le son de clic
  return stop;//Si on doit arrêter totalement le jeu on renvoie 1, si on redémarre juste le menu on renvoie -1
}

void colorButton(SDL_Surface*screen,SDL_Surface**button,int num,int x, int y){

  SDL_Rect pos;//Variable contenant la position du bouton que l'on va changer de couleur
  pos.x=x;//Position horizontal du bouton
  pos.y=y;//Position vertical du bouton

  SDL_BlitSurface(button[num],NULL,screen,&pos);//On place l'image en bleu du bouton

  SDL_Flip(screen);//On actualise le visuel du tout
  
}

int clickOnMenu(Mix_Chunk*sonClic,int x,int y){
  int ** tab=NULL; //On crée un pointeur sur un tableau contenant la grille de taquin
  int nbCoups; //Variable contenant le nombre de coups du joueur
  
  if (x>=80 && x<=400 && y>=80 && y<=144){ //Si il clique sur le bouton Nouvelle partie
    Mix_PlayChannel(-1,sonClic,0);//On active un son de clic
    tab=melanger(4);//On met dans tab une nouvelle grille mélangée de taquin
    jeu(tab,0);//lance le jeu de taquin avec la nouvelle grille et 0 en tant que nombre de coups
    return -1;//Renvoi -1 quand le jeu de taquin se termine de sorte à ce que le menu se relance entièrement
  }

  else if (x>=80 && x<=400 && y>=205 && y<=269){//Si il clique sur le bouton Recharger
    Mix_PlayChannel(-1,sonClic,0);//On lance le son de clic
    tab=(int**)malloc(4*sizeof(int*));//On alloue de la place pour la grille de 4*4
    for (int i=0;i<4;i++){
      tab[i]=(int*)malloc(4*sizeof(int));
    }
    nbCoups =recharger(tab,4); //On charge la grille du contenu du fichier de sauvegarde de grille et on stock le nombre de coups de la sauvegarde dans nbCoups
    jeu(tab,nbCoups);//On lance le jeu de taquin avec la grille rechargée et le nombre de coups correspondant
    return -1;//Renvoi -1 pour relancer le menu
    }

  else if (x>=80 && x<=400 && y>=380 && y<=444){//Si il clique sur le bouton Quitter
    Mix_PlayChannel(-1,sonClic,0);//On lance le son du clic
    return 1;//On renvoie 1 pour terminer totalement le jeu
  }
  
  else { //Si il clique à un endroit neutre
    return 0;//La boucle des event continue
  }
}

void jeu(int**tab,int nbCoups){

  SDL_Surface * screen=initialisation(2,900,480);//On initialise la fenêtre de jeu dans screen
  
  SDL_Surface ** rect=createRect(16,"rect/");//On fait pointer les surfaces de la liste rect sur les images des cases de la grille de taquin
  
  synchro(screen,rect,tab,nbCoups);//On synchronise l'affichage des images des cases avec la grille de taquin dans tab

  SDL_Surface ** button = createRect(12,"button/button");//On fait pointer les 12 surfaces de button sur es images des boutons Sauvegarder, Recharger et Quitter (12 car il y a les images des boutons en gris, bleu et bleu foncé)

  setButtonJeu(screen,button,tab);//On affiche correctement les images des boutons

  eventJeu(screen,rect,button,tab,nbCoups);//On lance la boucle qui gère les inputs du joueur durant le jeu de taquin

  liberer(tab,4); //Le jeu de taquin est terminé donc on libère la mémoire allouée pour le tableau
  freeSurface(button,12);//On libère la mémoire allouée pour la liste de surfaces pointant sur des images de boutons
  freeSurface(rect,16);//On libère la mémoire allouée pour la liste des images de la grille de taquin
  SDL_FreeSurface(screen); //On libère la mémoire allouée pour l'image de fond
  
}

void synchro(SDL_Surface*screen,SDL_Surface**rect,int**tab,int nbCoups){
  SDL_Rect pos;//Variable contenant les positions des différents éléments que l'on va positionner
  pos.x=0; //Coord axe horizontal
  pos.y=0;//Coord axe vertical
  TTF_Font * font = NULL;
  font = TTF_OpenFont("font/Adobe_Dia.ttf",41);//Pointeur pointant sur une police de taille 41
  if (font == NULL){ //Si font n'a pas réussi à pointer vers la police
    fprintf(stderr,"\nUnable to load TTF: %s\n",TTF_GetError() );//Message d'erreur
  }

  SDL_Surface * background = SDL_LoadBMP("image/background1.bmp");//Surface pointant vers une image de fond
  
  SDL_Color colorText;//couleur de la police (blanc)
  colorText.r = 255;
  colorText.g = 255;
  colorText.b = 255;



  char nbCoupsTxt[30];//Chaîne de caractère contenant le texte à écrire pour donner le nombre de coups

  sprintf(nbCoupsTxt,"Nombre de coups: %i",nbCoups);//On écrit à la fin du texte le nombre de coups

  SDL_Surface * nbCoupsTTF = TTF_RenderText_Blended(font,nbCoupsTxt,colorText); //Surface pointenant vers l'image du rendu du texte

  SDL_Surface * blocNbCoups = SDL_LoadBMP("image/nbCoups.bmp");//Surface pointant vers l'image de la case dans laquelle on va écrire le texte

  SDL_BlitSurface(background,NULL,screen,&pos);//On pose l'image de fond en [0;0] pour cacher l'écran de réussite du jeu de taquin au cas où il soit affiché

   pos.x=540;
  pos.y=62;
  //Coord de l'image de la case dans laquelle on va écrire le texte

  SDL_BlitSurface(blocNbCoups,NULL,screen,&pos);//On met l'image de la case aux dernières coordonnées
  
  pos.x=575;
  pos.y=76;
  //Coord de l'emplacement du texte
  SDL_BlitSurface(nbCoupsTTF,NULL,screen,&pos);//On place le texte au dernières coordonnées
  
  for (int i=0;i<4;i++){
    for (int j=0;j<4;j++){//Pour toutes les cases de la grille de taquin
      pos.x=60+95*j;
      pos.y=60+95*i;
      //Coord de la case [i;j]
      SDL_BlitSurface(rect[tab[i][j]-1],NULL,screen,&pos);//On place l'image de la case en fonction de la grille dans tab
    }
  }
  
  TTF_CloseFont(font); //On libère la mémoire allouée pour pointer sur la police
  SDL_FreeSurface(background); //On libère la mémoire allouée pour l'image de fond
  SDL_FreeSurface(nbCoupsTTF); //On libère la mémoire allouée pour l'image du texte

}

void setButtonJeu(SDL_Surface*screen,SDL_Surface**button,int **tab){

  SDL_Rect pos;//Variable contenant les positions des boutons que l'on va placer
  pos.x=540;//Coord axe horizontal
  pos.y=156;//coord axe vertical
  //Coord de l'image du bouton sauvegarder
  if (verifier(tab,4)==0){//Si le jeu de taquin n'est pas encore réussi. S'il est réussi on remettrait l'image de sauvegarder en gris alors qu'il serait censé être gris foncé
    SDL_BlitSurface(button[3],NULL,screen,&pos);//On met l'image de sauvegarder en gris
  }
  pos.y=pos.y+104;
  //Coord de l'image du bouton recharger

  SDL_BlitSurface(button[1],NULL,screen,&pos);//On place l'image du bouton Recharger en gris

  pos.y=pos.y+104;//Coord du bouton Quitter
  
  SDL_BlitSurface(button[2],NULL,screen,&pos);//On place l'image du bouton Quitter en gris

  SDL_Flip(screen);//On actualise le visuel
}

void eventJeu(SDL_Surface*screen,SDL_Surface**rect,SDL_Surface**button,int**tab,int nbCoups){
  int success=verifier(tab,4); //Variable contenant 1 si le jeu de taquin est réussi, 0 sinon
  int stop=0;//variable contenant 1 si on doit relancer un écran de menu
  int *nbCoupsPoint=&nbCoups; //On crée un pointeur vers le nombre de coups pour pouvoir le modifier dans les autres fonctions
  if (success==1){//Si le jeu est réussi
    achievement(screen); //On affiche l'écran de succès
  }
  SDL_Event event; //Variable contenant les inputs du joueur
  Mix_Chunk * sonPermu = Mix_LoadWAV("sound_effect/2.wav");//Pointeur vers l'effet sonore de permutation de case
  Mix_Chunk * sonClic = Mix_LoadWAV("sound_effect/1.wav");//Pointeur vers l'effet sonore de clic
  Mix_Chunk * sonSuccess = Mix_LoadWAV("sound_effect/3.wav");//Pointeur vers l'effet sonore de réussite du jeu de taquin
  int x,y;//Variables contenant la position de la souris quand on la déplace
  while(stop==0){//Tant que stop n'est pas à 1 on lit les inputs du joueur
    SDL_WaitEvent(&event);//Attente d'une entrée du joueur
    switch(event.type){//On étudie les cas suivants
    case SDL_QUIT: //Si le joueur ferme la fenêtre 
      stop=1;//le jeu de taquin s'arrête et on relance un écran de menu
      break;
    case SDL_KEYDOWN://S'il appuie sur une touche
      if ((success==0) | (verifier(tab,4)==0)){//S'il n'a pas encore réussi le jeu. On utilise l'opérateur "|" pour ne pas avoir à faire trop de calculs pour rien. Il est possible que success soit à 1 mais que le taquin ne soit pas réussi, dans le cas où on a rechargé une sauvegarde de grille alors qu'on vient de réussir le jeu
	success=0;//La grille n'est donc pas réussi
	switch(event.key.keysym.sym){
	case SDLK_LEFT: //Si la touche est la flèche de gauche
	  if (permuter(tab,'g',4)==1){//On permute la case vide avec la case juste à sa gauche si possible, et si possible on rentre dans le if
	    Mix_PlayChannel(-1,sonPermu,0);//On joue le son de permutation
	    *nbCoupsPoint = *nbCoupsPoint + 1;//Le nombre de coups augmente de 1
	    synchro(screen,rect,tab,*nbCoupsPoint);//On synchronise les positions des images des cases de la grille avec les cases de la grille
	    SDL_Flip(screen); //On actualise le visuel
	  }
	  break;
	case SDLK_RIGHT: //Si la flèche est la flèche de droite
	  if (permuter(tab,'d',4)==1){//On permute la case vide avec la case juste à sa droite si possible, et si possible on rentre dans le if
	      Mix_PlayChannel(-1,sonPermu,0);//On joue le son de permutation
	      *nbCoupsPoint = *nbCoupsPoint + 1;//Le nombre de coups augmente de 1
	      synchro(screen,rect,tab,*nbCoupsPoint);//On synchronise les positions des images des cases de la grille avec les cases de la grille 
	      SDL_Flip(screen); //On actualise le visuel
	  }
	  break;
	  case SDLK_UP://Si la touche est la flèche du haut
	  if (permuter(tab,'h',4)==1){//On permute la case vide avec la case juste en haut si possible, et si possible on rentre dans le if
	    Mix_PlayChannel(-1,sonPermu,0);//On joue le son de permutation
	    *nbCoupsPoint = *nbCoupsPoint + 1;//On incrémente de 1 le nombre de coups
	    synchro(screen,rect,tab,*nbCoupsPoint);//On synchronise les positions des images des cases de la grille avec les cases de la grille 
	    SDL_Flip(screen);//On actualise le visuel
	  }
	  break;
	  case SDLK_DOWN://Si la touche est la flèche du bas
	  if (permuter(tab,'b',4)==1){//On permute la case vide avec la case juste en bas si possible, et si possible on rentre dans le if
	    Mix_PlayChannel(-1,sonPermu,0);//On joue le son de permutation
	    *nbCoupsPoint = *nbCoupsPoint + 1;//On incrémente de 1 le nombre de coups
	    synchro(screen,rect,tab,*nbCoupsPoint);//On synchronise les positions des images des cases de la grille avec les cases de la grille
	    SDL_Flip(screen);//On actualise le visuel
	  }
	  break;
	default:
	break;
	}
	  if (verifier(tab,4)==1){//Si le taquin est réussi après la permutation qui vient d'être effectuée
	    achievement(screen);//On affiche l'écran de succès
	    Mix_PlayChannel(-1,sonSuccess,0);//On joue le son de succès
	    success=1;//On indique à la variable que le jeu de taquin a été réussi
	}
      }
      break;
      case SDL_MOUSEBUTTONDOWN://Si il clique sur un bouton de la souris
	if (event.button.button==SDL_BUTTON_LEFT){//Si il fait un clic gauche
	  stop=clickOnJeu(screen,rect,button,tab,nbCoupsPoint,sonClic,event.button.x,event.button.y);//On rentre dans stop la valeur retournée par la fonction qui gère les clic de la souris pour savoir si le jeu de taquin se termine 
      }
      break;
      case SDL_MOUSEBUTTONUP://Si on relâche un bouton de souris
	if (event.button.button==SDL_BUTTON_LEFT){//Si on relâche le clic gauche
	  x=event.motion.x;//Coord axe horizontal
	  y=event.motion.y;//Coord axe vertical
      //Position de la souris au relâchement du clic
      if (x>=540 && x<=860 && y>=156 && y<=220 && success==0){//Si on clique sur sauvegarder
	colorButton(screen,button,7,540,156);//On fait passer le bouton du bleu foncé au bleu
      }

      else if (x>=540 && x<=860 && y>=260 && y<=324){//Si on clique sur Recharger
	colorButton(screen,button,5,540,260);//On fait passer le bouton du bleu foncé au bleu
      }

      else if (x>=540 && x<=860 && y>=364 && y<=428){//Si on clique sur Quitter
	colorButton(screen,button,6,540,364);//On fait passer le bouton du bleu foncé au bleu
      }
      else {//Si on clic sur un endroit qui n'est pas un bouton
	setButtonJeu(screen,button,tab);//On réaffiche tous les boutons au cas où certains seraient affichés en bleu foncé
      }
      }
      break;
      case SDL_MOUSEMOTION://Si on bouge la souris
	x=event.motion.x;//Coord axe horizontal
	y=event.motion.y;//Coord axe vertical
      //Position de la souris
	if (x>=540 && x<=860 && y>=156 && y<=220 && success==0){//Si il passe sur le bouton Sauvegarder et que success est à 0, donc que le jeu n'est pas encore réussi
	  colorButton(screen,button,7,540,156);//On passe du gris au bleu sur ce bouton
      }

	else if (x>=540 && x<=860 && y>=260 && y<=324){//Si il passe sur le bouton Recharger
	  colorButton(screen,button,5,540,260);//On passe du gris au bleu sur ce bouton
      }

	else if (x>=540 && x<=860 && y>=364 && y<=428){//Si il passe sur le bouton Quitter
	colorButton(screen,button,6,540,364);//On passe du gris au bleu sur ce bouton
      }
	else {//S'il ne passe sur aucun bouton particulier
	  setButtonJeu(screen,button,tab);//On réaffiche tous les boutons au cas où certains seraient affichés en bleu.
      }
      break;
      default:
      break;
    }
  }
    Mix_FreeChunk(sonPermu); //On libère la mémoire allouée pour le son de permutation
    Mix_FreeChunk(sonClic); //On libère la mémoire allouée pour le son du clic
    Mix_FreeChunk(sonSuccess); //On libère la mémoire allouée pour le son de succès du jeu de taquin
}


int clickOnJeu(SDL_Surface*screen,SDL_Surface**rect,SDL_Surface**button,int **tab,int*nbCoups,Mix_Chunk*sonClic,int x, int y){
  
  if (x>=540 && x<=860 && y>=156 && y<=220 && verifier(tab,4)==0){ //Si il clique sur le bouton Sauvegarder
    Mix_PlayChannel(-1,sonClic,0); //On lance le son de clic
    sauvegarder(tab,*nbCoups,4); //On sauvegarde la grille de taquin ainsi que le nombre de coups passé en paramètre
    colorButton(screen,button,11,540,156); //On affiche le bouton Sauvegarder en bleu foncé (pressé)
    return 0; //On reste dans la boucle d'input du joueur
  }

  else if (x>=540 && x<=860 && y>=260 && y<=324){//Si il clique sur le bouton Recharger
    Mix_PlayChannel(-1,sonClic,0);//On joue le son de clic
    *nbCoups=recharger(tab,4); //On recharge la grille et on met dans la case mémoire du nombre de coups la valeur du nombre de coups sauvegardée

    synchro(screen,rect,tab,*nbCoups);//On synchronise les images des cases de la grille avec la grille
    setButtonJeu(screen,button,tab);//On remet tous les boutons en bleu au cas où le taquin soit auparavant réussi et que le bouton Sauvegarder soit bleu foncé
    
    if (verifier(tab,4)==1){//Si la grille rechargée était réussie
      achievement(screen);//On affiche l'écran de réussite
    }

    colorButton(screen,button,9,540,260);//On met en bleu foncé le bouton Recharger car il est pressé
    return 0;//On reste dans la boucle d'input du joueur
  }

  else if (x>=540 && x<=860 && y>=364 && y<=428){//Si il clique sur le bouton quitter
    Mix_PlayChannel(-1,sonClic,0);//On lance le son de clic
    return 1;//On sort de la boucle d'input du joueur dans le but de relancer un menu
  }
  else {//Si il ne clique sur aucun bouton
    return 0;//On reste dans la boucle d'input du joueur
  }

  
}

void achievement(SDL_Surface*screen){

  SDL_Surface * success = SDL_LoadBMP("image/achievement0.bmp");//Pointeur qui va pointer vers l'image correspondant au rectangle de succès
  
  SDL_Surface * background = SDL_LoadBMP("image/background1.bmp");//Pointeur qui va pointer vers l'image de fond

  SDL_Surface * sauvegarderOff=SDL_LoadBMP("image/sauvegarder.bmp");//Pointeur qui va pointer vers l'image du bouton sauvegarder grisée car inaccessible puisque le taquin est réussi
  
  SDL_Rect posBackground;//Variable comprenant la position de l'image de fond
  posBackground.x=0;//Coord axe horizontal
  posBackground.y=0;//Coord axe vertical
  
  SDL_Rect posSuccess;
  posSuccess.x=80;//Coord axe horizontal
  posSuccess.y=208;//Coord axe vertical

  SDL_Rect posSauvegarderOff;
  posSauvegarderOff.x=540;//Coord axe horizontal
  posSauvegarderOff.y=156;//Coord axe vertical
  
  SDL_BlitSurface(background,NULL,screen,&posBackground);//On pose l'image de fond à ses coordonnées
  SDL_BlitSurface(success,NULL,screen,&posSuccess);//On pose l'image du rectangle de succès à ses coordonnées
  SDL_BlitSurface(sauvegarderOff,NULL,screen,&posSauvegarderOff);//On pose l'image du bouton de sauvegarde à ses coordonnées

  SDL_Flip(screen);//On actualise le visuel

  SDL_FreeSurface(background);//On libère la mémoire allouée pour l'image de fond
  SDL_FreeSurface(success);//On libère la mémoire allouée pour l'image du rectangle de succès
  SDL_FreeSurface(sauvegarderOff);//On libère la mémoire allouée pour l'image du bouton de sauvegarde.

}

void freeSurface(SDL_Surface**rect,int n){
  for (int i=0;i<n;i++){ //Pour toutes les surfaces de la liste
    SDL_FreeSurface(rect[i]);//On libère la mémoire allouée pour ce qui est pointé par le pointeur de surface
  }
  free(rect);//On libère le la mémoire allouée par le pointeur sur les pointeur de surface.
}

