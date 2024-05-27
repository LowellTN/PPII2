#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>
#include "const.h"
#include "perso.h"
#include "textures.h"
#include "fonts.h"
#include "music.h"
#include <SDL2/SDL_mixer.h>
#include "init.h"
#include "menu.h"


void initSDL(SDL_Window **window, SDL_Renderer **renderer){
	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in init : %s", SDL_GetError());
		exit(-1);
	}

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Erreur lors de l'initialisation de SDL : %s", SDL_GetError());
        exit(-1);
    }

	*window = SDL_CreateWindow("SDL window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINWIDTH, WINHEIGHT, SDL_WINDOW_SHOWN);
	if (!window) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in window init : %s", SDL_GetError());
		exit(-1);
	}

	*renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in renderer init : %s", SDL_GetError());
		exit(-1);
	}
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
       printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
       exit(-1);
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 8, 2048) < 0) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        exit(-1);
    }
    TTF_Init();
    initCursorRects();
    load_music();
    loadTextures(*renderer);
    loadFonts();
}

void quitSDL(SDL_Renderer **renderer, SDL_Window **window,  Perso *perso, Map *map, Boss *boss){
    SDL_DestroyRenderer(*renderer);
    freeTextures();
    freeFonts();
    TTF_Quit();
    free(perso);
    SDL_DestroyWindow(*window);
    destroyMap(map);
    free_music();
    Mix_HaltMusic();
    Mix_CloseAudio();
}
