#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL_mixer.h>
#include "const.h"
#include "enemy1.h"
#include "boss.h"

void game(EnemyStateData enemyStateData, Boss *boss,Map *map, Perso *perso,const Uint8 *state, Mix_Chunk **sounds);
void game2(SDL_Renderer *renderer, PersoFight *playerInFight, bossFight *boss, AttackFight *attack1, AttackFight *attack2, AttackFight *attack3, AttackFight *attack4, AttackFight *attack5, AttackFight *attack6, Animation *scracthAnim, Animation *hitpointAnim);

#endif /* GAME_H */
