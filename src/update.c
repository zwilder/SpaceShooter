/*
* Space Shooter
* Copyright (C) Zach Wilder 2024
* 
* This file is a part of Space Shooter
*
* Space Shooter is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* Space Shooter is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with Space Shooter.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <spaceshooter.h>

void update_menu(WSL_App *game);
void update_newgame(WSL_App *game);
void update_game(WSL_App *game);
void update_scores(WSL_App *game);
void update_gameover(WSL_App *game);

void update(WSL_App *game) {
    switch(game->state) {
        case GS_MENU:
            update_menu(game);
            break;
        case GS_NEW:
            update_newgame(game);
            break;
        case GS_GAMEOVER:
            update_gameover(game);
            break;
        case GS_SCORES:
            update_scores(game);
            break;
        case GS_GAME:
            update_game(game);
            break;
        default:
            break;
    }
}

void update_menu(WSL_App *game) {
    Entity *entity = NULL, *tmp = NULL;

    // Update entities
    entity = game->entities;
    while(entity) {
        entity->update(entity,game);
        entity = entity->next;
    }
    
    // Cleanup entity list
    entity = game->entities;
    while(entity) {
        tmp = entity;
        entity = entity->next;
        if(tmp->cooldown) {
            tmp->cooldown -= 1;
        } else {
            tmp->flags &= ~EF_COOLDOWN;
        }
        if(!((tmp->flags & EF_ALIVE) == EF_ALIVE)) {
            //Entity is dead, call death function, remove it
            if(tmp->deathfunc) tmp->deathfunc(tmp, game);
            wsl_destroy_entity(game, tmp);
        }
    }

    // Check enemy spawn timer
    if(game->asteroidspawn) {
        game->asteroidspawn -= 1;
    } else {
        // Spawn asteroid, maybe
        spawn_asteroid(game);
    }

    // Advance background
    game->bgoffset += 4;
    if(game->bgoffset > game->bg->h) game->bgoffset = 0;
}

void update_newgame(WSL_App *game) {
    Entity *entity = NULL, *tmp = NULL;

    // Destroy all entities
    entity = game->entities;
    while(entity) {
        tmp = entity;
        entity = entity->next;
        wsl_destroy_entity(game, tmp);
    }

    // Make the player
    SDL_Rect playerrect = {211, 941, 99 ,75};
    Entity *player = create_player(playerrect);
    player->x = (SCREEN_WIDTH / 2) - (playerrect.w / 2);
    player->y = (SCREEN_HEIGHT) - playerrect.h;
    player->flags |= EF_INV | EF_COOLDOWN; // Start "invulnerable"
    player->frame = 60; //60fps, 120 is 2 seconds
    player->cooldown = 25; // Can't shoot while "invulnerable"
    //player->txt = "SPUDS"; //Can't assign strings this way, just a reminder to
                             //do this (correctly) someday
    wsl_add_entity(game, player);

    // Good luck!
    /* 
     * Span long lived blip that flickers, and a short steady one on top...
     * this gives the "neat" effect of a display "buzzing" out.
     */
    spawn_bliptxt(SCREEN_WIDTH/2 - (FONT_SIZE * 17)/2,
            SCREEN_HEIGHT/2 - FONT_SIZE,
            game,"G O O D   L U C K , C A D E T !",
            90,30,
            255,0,0,250);
    spawn_bliptxt(SCREEN_WIDTH/2 - (FONT_SIZE * 17)/2,
            SCREEN_HEIGHT/2 - FONT_SIZE,
            game,"G O O D   L U C K , C A D E T !",
            30,0,
            255,0,0,250);

    // Reset the score
    game->score = 0;

    // Switch the state
    game->state = GS_GAME;
}

void update_game(WSL_App *game) {
    Entity *entity = NULL, *tmp = NULL;

    // Update entities
    entity = game->entities;
    while(entity) {
        entity->update(entity,game);
        entity = entity->next;
    }

    // Cleanup entity list
    entity = game->entities;
    while(entity) {
        tmp = entity;
        entity = entity->next;
        if(tmp->cooldown) {
            tmp->cooldown -= 1;
        } else {
            tmp->flags &= ~EF_COOLDOWN;
        }
        if(!((tmp->flags & EF_ALIVE) == EF_ALIVE)) {
            //Entity is dead, call death function, remove it
            //if it's the player, change game state
            if(entity_is_player(tmp) && !entity_is_projectile(tmp)) {
                //Check player lives, subtract one if possible if not GAMEOVER
                game->state = GS_GAMEOVER;
                add_score(game, game->score);
                spawn_bliptxt(0,0,game," ", 60,0,0,0,0,0); // Slight pause bliptxt
            }
            if(tmp->deathfunc) tmp->deathfunc(tmp, game);
            wsl_destroy_entity(game, tmp);
        }
    }

    // Check enemy spawn timer
    if(game->asteroidspawn) {
        game->asteroidspawn -= 1;
    } else {
        // Spawn asteroid
        spawn_asteroid(game);
    }

    // Advance background
    game->bgoffset += 4;
    if(game->bgoffset > game->bg->h) game->bgoffset = 0;
}

void update_scores(WSL_App *game) {
    //Eventually this will do a fancy scrolling high scores list
    update_menu(game);
}

void update_gameover(WSL_App *game) {
    Entity *entity = NULL, *tmp = NULL;
    int i = 0;
    bool chstate = true;
    update_game(game); // Do all the same stuff as in game state but then...
    // Check to see if any keys were pressed
    // TODO: There needs to be some sort of delay or something here or a prompt
    // for a specific key on game over (otherwise this just poofs away if the
    // player is holding down a key when they die)
    //
    // Maybe - find a bliptxt entity, and if it exists, dont switch state?
    // sorta hacky but it works for now?
    for(i = 0; i < MAX_KEYBOARD_KEYS; i++) {
        if(game->keyboard[i]) {
            // Destroy all entities
            entity = game->entities;
            while(entity) {
                tmp = entity;
                entity = entity->next;
                if(!((tmp->flags & EF_ALIVE) == EF_ALIVE)) {
                    wsl_destroy_entity(game, tmp);
                }
                if(entity_is_blip(tmp)) {
                    chstate = false;
                }
            }
            if(chstate) {
                // Show the high score table!
                game->state = GS_SCORES;
            }
        }
    }
}

