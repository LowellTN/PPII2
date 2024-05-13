#include "main.h"
#include "checkpoints.h"
#include "const.h"
#include "dialog_box.h"
#include "enemy1.h"
#include "enemy2.h"
#include "enemy3.h"
#include "enemyBat.h"
#include "enemyFleche.h"
#include "fight.h"
#include "fonts.h"
#include "health.h"
#include "init.h"
#include "map.h"
#include "pendule.h"
#include "perso.h"
#include "scroll.h"
#include "textures.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL_mixer.h>
#include "music.h"

SDL_Texture *bgTextures[6];
SDL_Texture *tileTextures;

int main(int argc, char **argv) {

    SDL_Window *window;
    SDL_Renderer *renderer;
    initSDL(&window, &renderer);

    const SDL_Color RED = {.r = 255, .g = 0, .b = 0, .a = 0};

    Perso *playerInFight = (Perso *)malloc(sizeof(Perso));
    playerInFight->y = QUARTERHEIGHT - SPRITESIZE / 2;
    playerInFight->x = TIERWIDTH / 2 - SPRITESIZE / 2;
    Map *map = initMap("map2");
    Perso *perso = create_perso(map);

    SDL_Event event;
    int running = 1;

    loadBackgroundTextures(renderer, bgTextures, 5);
    loadTileTextures(renderer, &tileTextures, "./asset/tileset/ground-1.png");
    /* const SDL_Color BLACK = {.r = 0, .g = 0, .b = 0, .a = 255}; */ 

    while (running) {

        Uint64 start = SDL_GetTicks();

        if (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                running = 0;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_SPACE) {
                jump(perso, map);
                }
            }
        }

        perso->vx = 0;
        const Uint8 *state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_A])
            perso->vx -= MOOVSPEED;
        if (state[SDL_SCANCODE_D])
        perso->vx += MOOVSPEED;

            
            updatePerso(perso, map);
            updateCam(perso, map);

        if (drawBackground(renderer, bgTextures, 5, map)) {
            printf("Error drawing the background");
            exit(-1);
        }
        if (drawMap(renderer, map, "./asset/tileset/ground-1.png", tileTextures)) {
            printf("Error drawing the map");
            exit(-1);
        }
        // if (fightMovement(renderer, event, playerInFight)) {
        // 	printf("Error drawing the fight");
        // 	exit(-1);
        // }
        SDL_SetRenderDrawColor(renderer, RED.r, RED.g, RED.b, RED.a);
        if (display_perso(renderer, perso, map)) {
            printf("Error drawing the perso");
            exit(-1);
        }

        SDL_RenderPresent(renderer);

        Uint64 end = SDL_GetTicks();
        float elapsedMS = (end - start);
        SDL_Delay(fmaxf((1000 * DT - elapsedMS) / 1.0f, 0));
        atexit(SDL_Quit);
    }

    quitSDL(&renderer, &window, perso, map, playerInFight);
    atexit(SDL_Quit);
    return 0;
}
