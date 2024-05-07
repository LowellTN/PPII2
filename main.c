#include "main.h"
#include "checkpoints.h"
#include "const.h"
#include "dialog_box.h"
#include "enemy1.h"
#include "enemy2.h"
#include "enemy3.h"
#include "init.h"
#include "map.h"
#include "perso.h"
#include "scroll.h"
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
#include "pendule.h"
#include "textures.h"
#include "fonts.h"
#include "health.h"
#include "enemyFleche.h"
#include "enemyBat.h"
#include "fight.h"

SDL_Texture *bgTextures[6];
SDL_Texture *tileTextures;

int main(int argc, char **argv) {
	SDL_Window *window; SDL_Renderer *renderer;
	initSDL(&window, &renderer);

	// const SDL_Color BLACK = {.r = 0, .g = 0, .b = 0, .a = 255};
	// const SDL_Color WHITE = {.r = 255, .g = 255, .b = 255, .a = 255};
	const SDL_Color RED = {.r = 255, .g = 0, .b = 0, .a = 0};

	PersoFight *playerInFight = (PersoFight*)malloc(sizeof(PersoFight));
	playerInFight->y = QUARTERHEIGHT-1.5*SPRITESIZE/2;
	playerInFight->x = TIERWIDTH/2-1.5*SPRITESIZE/2;
	playerInFight->health = 10;
	playerInFight->iframe = 0;

	/* Création du boss */

	bossFight *boss = (bossFight*)malloc(sizeof(bossFight));
	boss->health = 9;
	boss->phase = 1;
	boss->delay = 200;
	boss->attack1Delay = 3*boss->delay;
	boss->attack2Delay = 3*boss->delay;
	boss->attack3Delay = 3*boss->delay;
	boss->speed = 2;

	/* Déclaration des attaques pour le gameplay 2 */

	AttackFight *nullAttack = initAttack(3*TIERWIDTH, 2*QUARTERHEIGHT, boss);
	AttackFight *attack1 = initAttack(0, 2*QUARTERHEIGHT, boss);
	AttackFight *attack2 = initAttack(TIERWIDTH, 2*QUARTERHEIGHT, boss);
	AttackFight *attack3 = initAttack(2*TIERWIDTH, 2*QUARTERHEIGHT, boss);
	AttackFight *attack4 = initAttack(0, 0, boss);
	AttackFight *attack5 = initAttack(TIERWIDTH, 0, boss);
	AttackFight *attack6 = initAttack(2*TIERWIDTH, 0, boss);
	
	int attackDelay1 = 3*boss->attack1Delay;

	Map *map = initMap("map1/data.txt");
	Perso *perso = create_perso(map);

	float x_cam = 0; // cam à gauche au début

	SDL_Event event;
	int running = 1;

	loadBackgroundTextures(renderer, bgTextures, 5);
	loadTileTextures(renderer, &tileTextures, "./asset/tileset/ground-1.png");

	while (running) {
		Uint64 start = SDL_GetTicks();

		if (SDL_PollEvent(&event)) {
			switch(event.type) {
			case SDL_QUIT :
				running = 0;
				break;
			case SDL_KEYDOWN :
				if (event.key.keysym.sym == SDLK_SPACE) {
					jump(perso, map);
				}
			}
		}

		perso->vx = 0;
		const Uint8 *state = SDL_GetKeyboardState(NULL);
		if (state[SDL_SCANCODE_A]) perso->vx -= MOVSPEED;
		if (state[SDL_SCANCODE_D]) perso->vx += MOVSPEED;

		updatePerso(perso, map);
		x_cam = updateCam(perso->x*PIX_RECT, x_cam);

		invincibility(playerInFight);

		if (drawBackground(renderer, bgTextures, 5, x_cam)) {
			printf("Error drawing the background");
			exit(-1);
		}
		if (drawMap(renderer, map, "./asset/tileset/ground-1.png", x_cam, tileTextures)) {
			printf("Error drawing the map");
			exit(-1);
		}
        if (fightBoss(renderer, boss, playerInFight,nullAttack, attack1, attack2, attack3, attack4, attack5, attack6)) {
            printf("Error playing boss fight");
			exit(-1);
		}
		if (fightMovement(renderer, event, playerInFight)) {
			printf("Error drawing the fight");
			exit(-1);
		}
		SDL_SetRenderDrawColor(renderer, RED.r, RED.g, RED.b, RED.a);
		if (display_perso(renderer, perso, x_cam)) {
			printf("Error drawing the perso");
			exit(-1);
		}

		// SDL_SetRenderDrawColor(renderer, WHITE.r, WHITE.g, WHITE.b, WHITE.a); // !!! seulement pour les tests de caméra (à changer)
		// SDL_Rect rect1 = {.x = x_perso*PIX_RECT - 10 - x_cam, .y = 3*PIX_RECT - 10, .w = 20, .h = 20}; // !!! seulement pour les tests de caméra (à changer)
		// SDL_RenderDrawRect(renderer, &rect1); // !!! seulement pour les tests de caméra (à changer)
		// SDL_SetRenderDrawColor(renderer, BLACK.r, BLACK.g, BLACK.b, BLACK.a); // !!! seulement pour les tests de caméra (à changer)
		// SDL_Rect rect2 = {.x = x_perso*PIX_RECT - 9 - x_cam, .y = 3*PIX_RECT - 9, .w = 18, .h = 18}; // !!! seulement pour les tests de caméra (à changer)
    	// SDL_RenderDrawRect(renderer, &rect2); // !!! seulement pour les tests de caméra (à changer)

	// printf("vie = %d\n", playerInFight->health);
	// printf("vie du boss = %d\n", boss ->health);
	// printf("phase = %d\n", boss->phase);

    SDL_RenderPresent(renderer);

    Uint64 end = SDL_GetTicks();
    float elapsedMS = (end - start);
    SDL_Delay(fmaxf((1000 * DT - elapsedMS) / 1.0f, 0));
  	}

	free(nullAttack);
	free(attack1);
	free(attack2);
	free(attack3);
	free(attack4);
	free(attack5);
	free(attack6);
    free(playerInFight);
	free(boss);
    quitSDL(&renderer, &window, perso, map);
    atexit(SDL_Quit);
    return 0;
}