#ifndef PERSO_H
#define PERSO_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include "const.h"
#include "map.h"
#include "enemy1.h"



Perso *create_perso(Map*);
void loadPersoTexture(SDL_Renderer*, SDL_Texture**, char*);
int display_perso(SDL_Renderer*, Perso*, Map*, SDL_Texture*, int);
int hitbox_bottom(Perso*, Map*);
int hitbox_top(Perso*, Map*);
int hitbox_left(Perso*, Map*);
int hitbox_right(Perso*, Map*);
float max(float, float);
float min(float, float);
void updatePerso(Perso*, Map*, EnemyStateData*, const Uint8*);
void jump(Perso*, Map*);
void changeGravity(void);

#endif
