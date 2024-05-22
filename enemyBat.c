#include "textures.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include "enemyBat.h"
/* #include "graph.c" */

/* s'utilise avec: */
  /* EnemyBatData enemyBatData; */
/*   initEnemyBat(&enemyBatData, xPos(à définir), yPos(à définir), xMax(à définir)); */
/* enemyBat_mouvement(renderer, &enemyBatData, map); */

void enemyBat_mouvement(SDL_Renderer *renderer, EnemyBatData *enemyBatData, Map *map) {
    int interval = 130;
    int intervalAttack = 1000;
    int speed = 20;
    double position_tolerance = 20;
    SDL_Rect dst_rect = {enemyBatData->dst_rect.x - map->x_cam, enemyBatData->dst_rect.y, enemyBatData->dst_rect.w, enemyBatData->dst_rect.h};
    SDL_Rect dst_rect2 = {enemyBatData->dst_rectAttack.x - map->x_cam, enemyBatData->dst_rectAttack.y, 64*2, enemyBatData->dst_rectAttack.h};
    SDL_Rect dst_rect2Ex = {enemyBatData->dst_rectAttack.x - map->x_cam - 25, enemyBatData->dst_rectAttack.y, 64*2, enemyBatData->dst_rectAttack.h};
    int pauseInterval = 1000;
    if (enemyBatData->state == BAT_MOVING_RIGHT){
            SDL_RenderCopyEx(renderer, textureBat, &enemyBatData->src_rect, &dst_rect, 0, NULL, SDL_FLIP_HORIZONTAL);
    }
    if (enemyBatData->state == BAT_MOVING_LEFT){
        SDL_RenderCopy(renderer, textureBat, &enemyBatData->src_rect, &dst_rect);
    }
    if (enemyBatData->state == BAT_ATTACK){
        if (enemyBatData->previousState == BAT_MOVING_LEFT){
            SDL_RenderCopy(renderer, textureBatAttack, &enemyBatData->src_rectAttack, &dst_rect2);
        }
        if (enemyBatData->previousState == BAT_MOVING_RIGHT){
            SDL_RenderCopyEx(renderer, textureBatAttack, &enemyBatData->src_rectAttack, &dst_rect2Ex, 0, NULL, SDL_FLIP_HORIZONTAL);
        }
    }
    switch(enemyBatData->state){
        case BAT_MOVING_RIGHT:
            if (SDL_GetTicks() - enemyBatData->pauseStartBits >= interval){
                enemyBatData->dst_rect.x += speed;
                enemyBatData->src_rect.x += 32;
                enemyBatData->pauseStartBits = SDL_GetTicks();
            }
            if (enemyBatData->src_rect.x == 160){
                enemyBatData->src_rect.x = 0;
            }
            if (fabs((double)enemyBatData->dst_rect.x - enemyBatData->xMax) <= position_tolerance){
                enemyBatData->state = BAT_MOVING_LEFT;
            }
            break;
        case BAT_MOVING_LEFT:
        if (fabs((double)enemyBatData->dst_rect.x - enemyBatData->xMin) <= position_tolerance){
            enemyBatData->state = BAT_MOVING_RIGHT;
        }
        if (SDL_GetTicks() - enemyBatData->pauseStartBits >= interval){
            enemyBatData->dst_rect.x -= speed;
            enemyBatData->src_rect.x += 32;
            enemyBatData->pauseStartBits = SDL_GetTicks();
        }
        if (enemyBatData->src_rect.x == 160){
            enemyBatData->src_rect.x = 0;
        }
        break;
        case BAT_ATTACK:
        enemyBatData->dst_rectAttack.x = enemyBatData->dst_rect.x;
            if (SDL_GetTicks() - enemyBatData->pauseStartBitsAttack >= interval){
                enemyBatData->src_rectAttack.x += 1*64;
                enemyBatData->pauseStartBitsAttack = SDL_GetTicks();
            }
            printf("x %d\n", enemyBatData->src_rectAttack.x);
            if (fabs((double)enemyBatData->src_rectAttack.x - 448) <= position_tolerance){
                enemyBatData->state = enemyBatData->previousState;
                enemyBatData->src_rectAttack.x = 0;
            }

        /* et le problème de musique pour tous les trucs */
        /* et les hitbox pour chaque personnage */

            break;
    }
    printf("state: %d\n", enemyBatData->state);

}


void initEnemyBat(EnemyBatData *enemyBatData, int x, int y, int xMax){

    enemyBatData->src_rect.x = 0;
    enemyBatData->src_rect.y = 0;
    enemyBatData->src_rect.w = 32;
    enemyBatData->src_rect.h = 64;

    enemyBatData->dst_rect.x = x;
    enemyBatData->dst_rect.y = y;
    enemyBatData->dst_rect.w = 32*2;
    enemyBatData->dst_rect.h = 64*2;

    enemyBatData->pauseStartBits = 0;
    enemyBatData->pauseStartBitsAttack = 0;
    enemyBatData->pauseAttack = 0;
    enemyBatData->state = BAT_MOVING_RIGHT;
    enemyBatData->xMax = xMax;
    enemyBatData->xMin = x;

    enemyBatData->src_rectAttack.x = 0; 
    enemyBatData->src_rectAttack.y = 0;
    enemyBatData->src_rectAttack.w = 64;
    enemyBatData->src_rectAttack.h = 64*2;

    enemyBatData->dst_rectAttack.x = x;
    enemyBatData->dst_rectAttack.y = y;
    enemyBatData->dst_rectAttack.w = 64*2;
    enemyBatData->dst_rectAttack.h = 64*2;
}


void batAttack(EnemyBatData *enemyBatData, Perso *perso, Map *map){
    int intervalAttack = 1000;
    int spriteLength = 64;
    float borneInf = enemyBatData->dst_rect.x;
    float borneSup = borneInf + spriteLength;
    float persoPositionX = perso->x * map->pix_rect;

    if (enemyBatData->state != BAT_ATTACK){
        enemyBatData->previousState = enemyBatData->state;
    }

    if (persoPositionX >= borneInf && persoPositionX <= borneSup){
        if (SDL_GetTicks() - enemyBatData->pauseAttack >= intervalAttack){
            perso->health -= 1;
            enemyBatData->pauseAttack = SDL_GetTicks();
            enemyBatData->pauseMusic = SDL_GetTicks();
            enemyBatData->state = BAT_ATTACK;
        }
    }
    /* if (SDL_GetTicks() - enemyBatData->pauseMusic >= intervalAttack){ */
    /*     Mix_HaltChannel(channelAttack); */
    /*     enemyBatData->pauseMusic = 0; */
    /* } */


       if (enemyBatData->state == BAT_ATTACK && SDL_GetTicks() - enemyBatData->pauseAttack >= intervalAttack) {
        enemyBatData->state = enemyBatData->previousState;
        enemyBatData->pauseAttack = SDL_GetTicks();
    }

}

void move_enemy_to_node(SDL_Renderer *renderer, EnemyBatData *enemyBatData, Node *node, Map *map) {
    enemyBatData->dst_rect.x = node->x * map->pix_rect;
    enemyBatData->dst_rect.y = node->y * map->pix_rect;
    printf("'%d, %d\n", node->x, node->y);
    SDL_Rect dst_rectFixed = {enemyBatData->dst_rect.x - map->x_cam, enemyBatData->dst_rect.y, enemyBatData->dst_rect.w, enemyBatData->dst_rect.h};
    SDL_RenderCopy(renderer, textureBat, &enemyBatData->src_rect, &dst_rectFixed);
}


/* void follow_path(SDL_Renderer *renderer, EnemyBatData *enemyBatData, Node path[150], int path_length, Map *map) { */
/*     int interval = 100; */
/*     int speed = 10; */

/*     if (path == NULL || enemyBatData == NULL) { */
/*         printf("Error: Null pointer detected\n"); */
/*         return; */
/*     } */

/*     SDL_Rect dst_rectFixed = {enemyBatData->dst_rect.x - map->x_cam, enemyBatData->dst_rect.y, enemyBatData->dst_rect.w, enemyBatData->dst_rect.h}; */
/*     if (enemyBatData->state == BAT_MOVING_RIGHT){ */
/*         SDL_RenderCopy(renderer, textureBat, &enemyBatData->src_rect, &dst_rectFixed); */
/*     } */
/*     for (int i = 0; i < 150; i++) { */
/*         /1* move_enemy_to_node(renderer, enemyBatData, &path[i], map); *1/ */
/*         switch(enemyBatData->state){ */
/*             case BAT_MOVING_RIGHT: */
/*                 if (SDL_GetTicks() - enemyBatData->pauseStartBits >= interval){ */
/*                     enemyBatData->dst_rect.x += speed; */
/*                     /1* enemyBatData->dst_rect.x = path[i].x * map->pix_rect; *1/ */
/*                     /1* enemyBatData->dst_rect.y = path[i].y * map->pix_rect; *1/ */
/*                     SDL_RenderCopy(renderer, textureBat, &enemyBatData->src_rect, &dst_rectFixed); */
/*                     enemyBatData->pauseStartBits = SDL_GetTicks(); */
/*                 } */
/*         } */
/*     } */
/* } */


void follow_path(SDL_Renderer *renderer, EnemyBatData *enemyBatData, Node path[MAX_NODES], Map *map, Perso *perso) {
    int interval = 130;   // Animation interval in milliseconds
    int speed = 32;       // Movement speed
    SDL_Rect dst_rectFixed = {enemyBatData->dst_rect.x - map->x_cam, enemyBatData->dst_rect.y, enemyBatData->dst_rect.w, enemyBatData->dst_rect.h};
    int path_length = len_nodes(path);

    // Find the current target node in the path
    int i;
    for (i = 0; i < path_length; i++) {
        if ((enemyBatData->dst_rect.x < path[i].x * map->pix_rect && enemyBatData->state == BAT_MOVING_RIGHT) ||
            (enemyBatData->dst_rect.x > path[i].x * map->pix_rect && enemyBatData->state == BAT_MOVING_LEFT)) {
            break;
        }
    }

    // Play sound effect and adjust volume based on distance to the player
    int channel = Mix_PlayChannel(-1, musicEnemyBat, 0);
    double distance = fabs(perso->x * map->pix_rect - enemyBatData->dst_rect.x);
    if (distance > MAX_HEARING_DISTANCE) {
        Mix_Volume(channel, MIN_VOLUME);
    } else {
        double attenuation = 5.0 / (distance - 100); // Adjust the attenuation formula as needed
        int volume = (int)(MAX_VOLUME * attenuation);
        Mix_Volume(channel, volume);
        printf("Adjusting volume to %d on channel %d based on distance %f\n", volume, channel, distance);
    }

    // Render the bat sprite based on its state (direction)
    if (enemyBatData->state == BAT_MOVING_RIGHT) {
        SDL_RenderCopyEx(renderer, textureBat, &enemyBatData->src_rect, &dst_rectFixed, 0, NULL, SDL_FLIP_HORIZONTAL);
    } else if (enemyBatData->state == BAT_MOVING_LEFT) {
        SDL_RenderCopy(renderer, textureBat, &enemyBatData->src_rect, &dst_rectFixed);
    }

    // Calculate the difference in position to the current target node
    int dx = path[i].x * map->pix_rect - enemyBatData->dst_rect.x;
    int dy = path[i].y * map->pix_rect - enemyBatData->dst_rect.y;
    printf("dx: %d\n", dx);

    // Move the bat towards the target node
    if (SDL_GetTicks() - enemyBatData->pauseStartBits >= interval) {
        if (enemyBatData->state == BAT_MOVING_RIGHT) {
            if (enemyBatData->dst_rect.x / map->pix_rect >= path[i].x) {
                enemyBatData->state = BAT_MOVING_LEFT;
            }
            if (abs(dx) >= speed) {
                enemyBatData->dst_rect.x += speed;
            } else {
                enemyBatData->dst_rect.x = path[i].x * map->pix_rect;
            }
            if (abs(dy) >= speed) {
                enemyBatData->dst_rect.y += speed * (dy > 0 ? 1 : -1);
            } else {
                enemyBatData->dst_rect.y = path[i].y * map->pix_rect;
            }
        } else if (enemyBatData->state == BAT_MOVING_LEFT) {
            if (abs(dx) >= speed) {
                enemyBatData->dst_rect.x -= speed;
            } else {
                enemyBatData->dst_rect.x = path[i].x * map->pix_rect;
            }
            if (abs(dy) >= speed) {
                enemyBatData->dst_rect.y += speed * (dy > 0 ? 1 : -1);
            } else {
                enemyBatData->dst_rect.y = path[i].y * map->pix_rect;
            }
        }

        // Update animation frame
        enemyBatData->src_rect.x += 32;
        if (enemyBatData->src_rect.x >= 160) {
            enemyBatData->src_rect.x = 0;
        }
        enemyBatData->pauseStartBits = SDL_GetTicks();
    }
}
void follow_path2(SDL_Renderer *renderer, EnemyBatData *enemyBatData, Node **graph, Map *map, Perso *perso) {
    int interval = 130;   // Animation interval in milliseconds
    int speed = 32;       // Movement speed
    Node *goal = &graph[2][144];
    Node *start = &graph[8][1];
    Node *path = a_star(graph, map, goal, start);
    SDL_Rect dst_rectFixed = {enemyBatData->dst_rect.x - map->x_cam, enemyBatData->dst_rect.y, enemyBatData->dst_rect.w, enemyBatData->dst_rect.h};
    int path_length = len_nodes(path);

    // Find the current target node in the path
    int i;
    for (i = 0; i < path_length; i++) {
        if ((enemyBatData->dst_rect.x < path[i].x * map->pix_rect && enemyBatData->state == BAT_MOVING_RIGHT) ||
            (enemyBatData->dst_rect.x > path[i].x * map->pix_rect && enemyBatData->state == BAT_MOVING_LEFT)) {
            break;
        }
    }

    // Play sound effect and adjust volume based on distance to the player
    int channel = Mix_PlayChannel(-1, musicEnemyBat, 0);
    double distance = fabs(perso->x * map->pix_rect - enemyBatData->dst_rect.x);
    if (distance > MAX_HEARING_DISTANCE) {
        Mix_Volume(channel, MIN_VOLUME);
    } else {
        double attenuation = 5.0 / (distance - 100); // Adjust the attenuation formula as needed
        int volume = (int)(MAX_VOLUME * attenuation);
        Mix_Volume(channel, volume);
        printf("Adjusting volume to %d on channel %d based on distance %f\n", volume, channel, distance);
    }

    // Render the bat sprite based on its state (direction)
    if (enemyBatData->state == BAT_MOVING_RIGHT) {
        SDL_RenderCopyEx(renderer, textureBat, &enemyBatData->src_rect, &dst_rectFixed, 0, NULL, SDL_FLIP_HORIZONTAL);
    } else if (enemyBatData->state == BAT_MOVING_LEFT) {
        SDL_RenderCopy(renderer, textureBat, &enemyBatData->src_rect, &dst_rectFixed);
    }

    // Calculate the difference in position to the current target node
    int dx = path[i].x * map->pix_rect - enemyBatData->dst_rect.x;
    int dy = path[i].y * map->pix_rect - enemyBatData->dst_rect.y;
    printf("dx: %d\n", dx);

    // Move the bat towards the target node
    if (SDL_GetTicks() - enemyBatData->pauseStartBits >= interval) {
        if (enemyBatData->state == BAT_MOVING_RIGHT) {
            if (enemyBatData->dst_rect.x / map->pix_rect >= path[i].x) {
                enemyBatData->state = BAT_MOVING_LEFT;
            }
            if (abs(dx) >= speed) {
                enemyBatData->dst_rect.x += speed;
            } else {
                enemyBatData->dst_rect.x = path[i].x * map->pix_rect;
            }
            if (abs(dy) >= speed) {
                enemyBatData->dst_rect.y += speed * (dy > 0 ? 1 : -1);
            } else {
                enemyBatData->dst_rect.y = path[i].y * map->pix_rect;
            }
        } else if (enemyBatData->state == BAT_MOVING_LEFT) {
            if (abs(dx) >= speed) {
                enemyBatData->dst_rect.x -= speed;
            } else {
                enemyBatData->dst_rect.x = path[i].x * map->pix_rect;
            }
            if (abs(dy) >= speed) {
                enemyBatData->dst_rect.y += speed * (dy > 0 ? 1 : -1);
            } else {
                enemyBatData->dst_rect.y = path[i].y * map->pix_rect;
            }
        }

        // Update animation frame
        enemyBatData->src_rect.x += 32;
        if (enemyBatData->src_rect.x >= 160) {
            enemyBatData->src_rect.x = 0;
        }
        enemyBatData->pauseStartBits = SDL_GetTicks();
    }
    free(path);
}
