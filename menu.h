#ifndef MENU_H
#define MENU_H
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "const.h"
#include "init.h"
#include "boss.h"
#include "projectile.h"
#include "textures.h"
#include "map.h"
#include "perso.h"
#include "boss.h"
#include <string.h>

void loadSounds(Mix_Chunk **sounds);
void initCursorRects(void);
void renderSprite(SDL_Renderer *renderer);
void renderImage(SDL_Renderer *renderer, const char* imagePath, int x, int y, int width, int height);
bool loadMusicGameplay1(void);
bool loadMusicGameplay2(void);
void playMusic(Mix_Music* music);
void closeSDL_mixer(void);
void toggleMusic(void);
void handleVolumeCursor(SDL_Event *e);
void drawMenu(SDL_Renderer *renderer);
void drawMapMenu(SDL_Renderer *renderer);
void interactionMenu(SDL_Renderer *renderer);
void interactionPauseJeu(SDL_Renderer *renderer, Map **map2, Map **mapBoss, Perso **perso, Boss **boss);
void resetGame(SDL_Window **window, SDL_Renderer **renderer, Map **map2, Map **MapBoss, Perso **perso, Boss **boss);
void gameOver(SDL_Renderer *renderer, SDL_Texture*[], int layer, Map*, char);

#endif /* MENU_H */
