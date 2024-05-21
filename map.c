#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "const.h"
#include "map.h"

Map *initMap(char *name) {
	char *str_data = malloc(strlen(name) + 10);
	char *str_metadata = malloc(strlen(name) + 15);
	strcpy(str_data, name);
	strcpy(str_metadata, name);
	strcat(str_data, "/data.txt");
	strcat(str_metadata, "/metadata.txt");
	Map *res = malloc(sizeof(Map));
	res->x_cam = 0;

	int n_cp; // Nombre de checkpoints (pas encore utilisé)
	FILE *fmetadata = fopen(str_metadata, "r");
	if (fscanf(fmetadata, "%d %d %d %d %d %d %d", &res->height, &res->width, &res->start_x, &res->start_y, &res->end_x, &res->end_y, &n_cp) == 0) {
		printf("Error in metadata reading");
	}
	res->pix_rect = WINHEIGHT/res->height;

	FILE *fdata = fopen(str_data, "r");
	res->matrix = malloc(res->height*sizeof(char*));
    for (int i = 0; i < res->height; i++) {
		res->matrix[i] = malloc(res->width*sizeof(char) + 1);
        if (fgets(res->matrix[i], res->width + 1, fdata) == 0) {
			printf("String length is null");
		}
	}

	fclose(fmetadata);
    fclose(fdata);
	free(str_data);
	free(str_metadata);
	return res;
}


void updateCam(Perso *perso, Map *map) {
	float x_perso = perso->x * map->pix_rect;
	if (x_perso - map->x_cam > 0.6*WINWIDTH) {
		map->x_cam = x_perso - 0.6*WINWIDTH;
	}
	if (x_perso - map->x_cam < 0.4*WINWIDTH) {
		map->x_cam = x_perso - 0.4*WINWIDTH;
	}
	if (map->x_cam < 0) {
		map->x_cam = 0;
	}
	if (map->x_cam > map->width*map->pix_rect - WINWIDTH) {
		map->x_cam = map->width*map->pix_rect - WINWIDTH;
	}
}

// Fonction d'affichage du background

void loadBackgroundTextures(SDL_Renderer *renderer, SDL_Texture *bgTextures[], int layer) {
    int index = layer;
	for (int i = 0; i < layer; ++i) {
        char imagePath[100];
        snprintf(imagePath, 100, "./asset/background/Foret/plan-%d.png", index-i);
        bgTextures[i] = IMG_LoadTexture(renderer, imagePath);
        if (!bgTextures[i]) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error loading background texture %d: %s", i + 1, SDL_GetError());
            exit(-1);
        }
    }
}


int drawBackground(SDL_Renderer *renderer, SDL_Texture *bgTextures[], int layer, Map *map) {
	for (int i = 0; i < layer; ++i) {
		float parallax = (float)i / (layer - 1);
		int textureWidth;
        SDL_QueryTexture(bgTextures[i], NULL, NULL, &textureWidth, NULL);
		int repeats = (WINWIDTH / textureWidth) + 3;
		for (int j = 0; j < repeats; ++j) {
			int x_position = j * textureWidth - parallax * map->x_cam;
            if (x_position + textureWidth < 0 || x_position > WINWIDTH) continue;
			SDL_Rect bgRect = {.x = j * textureWidth - parallax * map->x_cam, .y = 0, .w = textureWidth, .h = WINHEIGHT};
			SDL_RendererFlip flip = (j % 2 == 0) ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
			if (SDL_RenderCopyEx(renderer, bgTextures[i], NULL, &bgRect, 0.0, NULL, flip)) {
				SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error rendering background texture %d: %s", i + 1, SDL_GetError());
				exit(-1);
			}
		}
	}
	return 0;
}

// Fonction d'affichage des tiles

void loadTileTextures(SDL_Renderer *renderer, SDL_Texture **tileTexture, char *tilePath) {
	SDL_Surface *tileSurface = IMG_Load(tilePath);
	if (!tileSurface) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in tile surface init %s", IMG_GetError());
		exit(-1);
	}
	*tileTexture = SDL_CreateTextureFromSurface(renderer, tileSurface);
	if (!*tileTexture) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in tile texture init: %s", SDL_GetError());
		exit(-1);
	}
	SDL_FreeSurface(tileSurface);
}


void display_tile(SDL_Renderer *renderer, int xoffset, int yoffset, int xpos, int ypos , Map *map, SDL_Texture *tileTexture) {
	int tilesetWidth, tilesetHeight;
	if (SDL_QueryTexture(tileTexture,NULL, NULL, &tilesetWidth, &tilesetHeight)) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,"Error in query tile texture: %s", SDL_GetError());
		exit(-1);
	}

	int tileWidth = tilesetWidth / 5;
	int tileHeight = tilesetHeight / 5;

	SDL_Rect tileRect = {.x = xoffset*tileWidth, .y = yoffset*tileHeight, .w = tileWidth, .h = tileHeight};
	SDL_Rect destRect = {.x = xpos*map->pix_rect - map->x_cam, .y = ypos*map->pix_rect, .w = map->pix_rect, .h = map->pix_rect};

	if (SDL_RenderCopy(renderer, tileTexture, &tileRect, &destRect)) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in render copy: %s", SDL_GetError());
	}
}

// Fonction d'affichage de la map

int drawMap(SDL_Renderer *renderer, Map *map, SDL_Texture *tileTexture) {
	for (int i = 0 ; i < map->height ; i++) {
		for (int j = 0 ; j < map->width ; j++) {
			if (j*map->pix_rect - map->x_cam > -map->pix_rect && j*map->pix_rect - map->x_cam < WINWIDTH) {
				if (map->matrix[i][j] == '7') {
					display_tile(renderer,0,2,j,i, map, tileTexture); // Coin superieur gauche
				}
				if (map->matrix[i][j] == '4') {
					display_tile(renderer,0,3,j,i, map, tileTexture); // Mur face gauche
				}
				if (map->matrix[i][j] == '1') {
					display_tile(renderer,0,4,j,i, map, tileTexture); // Coin inferieur gauche
				}
				if (map->matrix[i][j] == '9') {
					display_tile(renderer,4,2,j,i, map, tileTexture); // Coin superieur droit
				}
				if (map->matrix[i][j] == '6') {
					display_tile(renderer,4,3,j,i, map, tileTexture); // Mur face droit 
				}
				if (map->matrix[i][j] == '3') {
					display_tile(renderer,4,4,j,i, map, tileTexture); // Coin inferieur droit
				}
				if (map->matrix[i][j] == '8') {
					if (j%3==0) {
						display_tile(renderer,1,2,j,i, map, tileTexture);
					} else if (j%3==1) {
						display_tile(renderer,2,2,j,i, map, tileTexture);
					} else {
						display_tile(renderer,3,2,j,i, map, tileTexture); // Sol
					}
				}
				if (map->matrix[i][j] == '5') {
					display_tile(renderer,1,3,j,i, map, tileTexture); // Vide
				}
				if (map->matrix[i][j] == '2') {
					if (j%3==0) {
						display_tile(renderer,1,4,j,i, map, tileTexture);
					} else if (j%3==1) {
						display_tile(renderer,2,4,j,i, map, tileTexture);
					} else {
						display_tile(renderer,3,4,j,i, map, tileTexture);// Plafond
					}
				}
				if (map->matrix[i][j] == 'u') {
					display_tile(renderer,0,1,j,i, map, tileTexture); // Angle superieur droit
				}
				if (map->matrix[i][j] == 'i') {
					display_tile(renderer,1,1,j,i, map, tileTexture); // Angle inferieur droit
				}
				if (map->matrix[i][j] == 'o') {
					display_tile(renderer,2,1,j,i, map, tileTexture); // Angle inferieur gauche
				}
				if (map->matrix[i][j] == 'p') {
					display_tile(renderer,3,1,j,i, map, tileTexture); // Angle superieur gauche
				}
				if (map->matrix[i][j] == 'a') {
					display_tile(renderer,0,0,j,i, map, tileTexture); // Plateforme extremite droit
				}
				if (map->matrix[i][j] == 'z') {
					if (j%3==0) {
						display_tile(renderer,1,0,j,i, map, tileTexture);
					} else if (j%3==1) {
						display_tile(renderer,2,0,j,i, map, tileTexture);
					} else {
						display_tile(renderer,3,0,j,i, map, tileTexture);// Plafond
					} // Plateforme corps
				}
				if (map->matrix[i][j] == 'e') {
					display_tile(renderer,4,0,j,i, map, tileTexture); // Plateforme extremite gauche
				}
			}
		}
	}
	return 0 ;
}

void destroyMap(Map *map) {
	for (int i = 0; i < map->height; i++) {
		free(map->matrix[i]);
	}
	free(map->matrix);
	free(map);
}

