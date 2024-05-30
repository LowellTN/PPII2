#ifndef enemyFleche_h
#define enemyFleche_h
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include "perso.h"
#define INIT_ENEMYFLECHE(enemy, map, x, y) initEnemyFleche(enemy, (x) * (map)->pix_rect, (y) * (map)->pix_rect)


enum EnemyFlecheState{
    FLECHE_MOVING_UP,
    FLECHE_MOVING_DOWN,
    FLECHE_PAUSE_TOP,
    FLECHE_PAUSE_BOTTOM

};

struct _EnemyFlecheData{
    enum EnemyFlecheState state;
    SDL_Rect src_rect;
    SDL_Rect dst_rect;
    Uint32 pauseStartBits;
    Uint32 pauseStart;
    Uint32 pauseAttack;
    int attackCounter;
    Uint32 pauseMusic;

};
typedef struct _EnemyFlecheData EnemyFlecheData;
void initEnemyFleche(EnemyFlecheData *enemyFlecheData, int x, int y);
void flecheAttack(EnemyFlecheData *enemyFlecheData, Perso *perso, Map *map);
void enemyFleche_mouvement(SDL_Renderer *renderer, EnemyFlecheData *enemyFlecheData, Map *map);
int hitbox_enemyFleche(Perso *perso, Map *map, EnemyFlecheData *enemy);
void updatePersoEnemyFleche(Perso *perso, Map *map, EnemyFlecheData *enemy);

#endif
