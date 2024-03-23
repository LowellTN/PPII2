#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "const.h"


Map *init_map(char *name) {
	Map *res = malloc(sizeof(Map)) ;
	FILE *f = fopen(name, "r") ;
    // char buffer[100];
    for (int i = 0 ; i < HEIGHT ; i++) {
        fgets(res->matrix[i], 49, f) ;
	}
    fclose(f) ;
	res->start_x = 4.0*PIX_RECT ;
	res->start_y = 4.0*PIX_RECT ;
	return res ;
}


void display_tile(SDL_Renderer *renderer,int xoffset, int yoffset, int xpos,int ypos ,char *ImagePath) {
	SDL_Surface *tileSurface = IMG_Load(ImagePath);
	if (!tileSurface) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in map tile surface init %s", IMG_GetError());
		exit(-1);
	}
	SDL_Texture *tileTexture = SDL_CreateTextureFromSurface(renderer,tileSurface);
	if (!tileTexture) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in sprite texture init: %s", SDL_GetError());
		exit(-1);
	}
	SDL_FreeSurface(tileSurface);

	int tilesetWidth, tilesetHeight;
	if (SDL_QueryTexture(tileTexture,NULL, NULL, &tilesetWidth, &tilesetHeight)) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,"Error in query texture: %s", SDL_GetError());
		exit(-1);
	}

	int tileWidth = tilesetWidth / 5;
	int tileHeight = tilesetHeight / 5;

	SDL_Rect tileRect = {.x = xoffset*tileWidth, .y = yoffset*tileHeight, .w = tileWidth, .h = tileHeight};
	SDL_Rect destRect = {.x = xpos*PIX_RECT, .y = ypos*PIX_RECT, .w = PIX_RECT, .h = PIX_RECT};

	if (SDL_RenderCopy(renderer, tileTexture, &tileRect, &destRect)) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in render copy: %s", SDL_GetError());
	}
}

int draw_map(SDL_Renderer *renderer, Map *map, char *ImagePath) {
	for (int i = 0 ; i < HEIGHT ; i++) {
		for (int j = 0 ; j < WIDTH ; j++) {
			if (map->matrix[i][j] == '7') {
				display_tile(renderer,0,2,j,i,ImagePath); // Coin superieur gauche
			}
			if (map->matrix[i][j] == '4') {
				display_tile(renderer,0,3,j,i,ImagePath); // Mur face gauche
			}
			if (map->matrix[i][j] == '1') {
				display_tile(renderer,0,4,j,i,ImagePath); // Coin inferieur gauche
			}
			if (map->matrix[i][j] == '9') {
				display_tile(renderer,4,2,j,i,ImagePath); // Coin superieur droit
			}
			if (map->matrix[i][j] == '6') {
				display_tile(renderer,4,3,j,i,ImagePath); // Mur face droit 
			}
			if (map->matrix[i][j] == '3') {
				display_tile(renderer,4,4,j,i,ImagePath); // Coin inferieur droit
			}
			if (map->matrix[i][j] == '8') {
				display_tile(renderer,1,2,j,i,ImagePath); // Sol
			}
			if (map->matrix[i][j] == '5') {
				display_tile(renderer,1,3,j,i,ImagePath); // Vide
			}
			if (map->matrix[i][j] == '2') {
				display_tile(renderer,1,4,j,i,ImagePath); // Plafond
			}
			if (map->matrix[i][j] == 'u') {
				display_tile(renderer,0,1,j,i,ImagePath); // Angle superieur droit
			}
			if (map->matrix[i][j] == 'i') {
				display_tile(renderer,1,1,j,i,ImagePath); // Angle inferieur droit
			}
			if (map->matrix[i][j] == 'o') {
				display_tile(renderer,2,1,j,i,ImagePath); // Angle inferieur gauche
			}
			if (map->matrix[i][j] == 'p') {
				display_tile(renderer,3,1,j,i,ImagePath); // Angle superieur gauche
			}
			if (map->matrix[i][j] == 'a') {
				display_tile(renderer,0,0,j,i,ImagePath); // Plateforme extremite droit
			}
			if (map->matrix[i][j] == 'z') {
				display_tile(renderer,1,0,j,i,ImagePath); // Plateforme corps
			}
			if (map->matrix[i][j] == 'e') {
				display_tile(renderer,4,0,j,i,ImagePath); // Plateforme extremite gauche
			}
		}
	}
	return 0 ;
}
