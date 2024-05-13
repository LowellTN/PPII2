#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "map.h"



enum EnemyPenduleState{
    PENDULE_MOVING_UP,
    PENDULE_MOVING_DOWN,

};

struct _EnemyPenduleData{
    enum EnemyPenduleState state;
    SDL_Rect src_rect;
    SDL_Rect dst_rect;
    Uint32 pauseStartBits;
    Uint32 pauseStart;
    Uint32 pauseAttack;
    int attackCounter;
    Uint32 pauseMusic;

};

typedef struct _EnemyPenduleData EnemyPenduleData;
void enemyPendule_mouvement(SDL_Renderer *renderer, EnemyPenduleData *enemyFlecheData, Map *map);
void initEnemyPendule(EnemyPenduleData *enemyPenduleData, int x, int y);

