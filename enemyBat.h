#ifndef enemybat_h
#define enemybat_h
#include "enemyFleche.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>

enum EnemyBatState{
    BAT_MOVING_RIGHT,
    BAT_MOVING_LEFT
};

struct _EnemyBatData{
    enum EnemyBatState state;
    SDL_Rect src_rect;
    SDL_Rect dst_rect;
    Uint32 pauseStartBits;
    int xMax;
    int xMin;
};
typedef struct _EnemyBatData EnemyBatData;
void initEnemyBat(EnemyBatData *enemyBatData, int x, int y, int xMax);
void enemyBat_mouvement(SDL_Renderer *renderer, EnemyBatData *enemyBatData, float x_cam);

#endif
