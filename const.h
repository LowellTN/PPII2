#include <SDL2/SDL.h>
#ifndef CONST_H
#define CONST_H

#define WINHEIGHT 720 // en pixels
#define WINWIDTH 1280 // en pixels
#define HEIGHT 8 // i, y en nombre de tiles
#define WIDTH 48 // j, x en nombre de tiles
#define PIX_RECT WINHEIGHT/HEIGHT // en pixels
#define LEVEL_WIDTH PIX_RECT*WIDTH // en pixels
#define TIERWIDTH WINWIDTH/3
#define QUARTERHEIGHT WINHEIGHT/4
#define SPRITESIZE 64 // 64x64 taille des sprites de base
#define PERSO_WIDTH 60
#define PERSO_HEIGHT 60
#define ACC 25.5f
#define DT 1/60.0f
#define JUMPSPEED 11.0f
#define MOVSPEED 7.0f

typedef struct _Map {
    char matrix[HEIGHT][WIDTH] ;
    float start_x ;
    float start_y ;
} Map ;


typedef struct _Perso {
    float x; // en nombre de tiles
    float y; // en nombre de tiles
    float vx; // en nombre de tiles par seconde
    float vy; // en nombre de tiles par seconde
    int health; //je l'ai rajouté
    SDL_Rect hitbox;
} Perso;


#endif