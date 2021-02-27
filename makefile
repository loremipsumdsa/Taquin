taquin:main.o algo.o graphique.o
	gcc -o taquin main.o algo.o graphique.o `sdl-config --cflags --libs` -lSDL -l SDL_ttf -l SDL_mixer

main.o:main.c algo.o graphique.o
	gcc -c main.c -Wall -Wextra -o main.o `sdl-config --cflags --libs` -lSDL -l SDL_ttf -l SDL_mixer

algo.o:algo.c
	gcc -c algo.c -Wall -Wextra -o algo.o

graphique.o:graphique.c algo.h
	gcc -c graphique.c -Wall -Wextra -o graphique.o `sdl-config --cflags --libs` -lSDL -l SDL_ttf -l SDL_mixer