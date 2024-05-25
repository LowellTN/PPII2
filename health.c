#include "health.h"
#include "const.h"
#include "textures.h"
#include "boss.h"
#include <SDL2/SDL_render.h>
#include "checkpoints.h"

void renderStatusHealth(SDL_Renderer *renderer, Perso *perso){
    SDL_Rect dst_rect = {-10, -80, 128*2, 128*2};
    if (perso->health >= 0 && perso->health <= 9){
        SDL_RenderCopy(renderer, healthTextures[perso->health], NULL, &dst_rect);
    }
}

void renderStatusHealthFight(SDL_Renderer *renderer, PersoFight *perso){
    SDL_Rect dst_rect = {-10, -80, 128*2, 128*2};
    if (perso->health >= 0 && perso->health <= 9){
        SDL_RenderCopy(renderer, healthTextures[perso->health], NULL, &dst_rect);
    }
}

void renderStatusBoss(SDL_Renderer *renderer, Boss *boss) {
    SDL_Rect dst_rect = {990, -80, 128*2, 128*2};
    if (boss->health >= 0 && boss->health <= 9){
        SDL_RenderCopy(renderer, healthTextures[boss->health], NULL, &dst_rect);
    }
}

void revive(Perso *perso) {
    perso->health = 9;
}

