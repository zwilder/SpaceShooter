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

void update(WSL_App *game) {
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
            //Entity is dead, remove it
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

void update_player(Entity *player, WSL_App *game) {
    Entity *proj = NULL;
    SDL_Rect projrect = {845,0,13,57};
    player->dx = player->dy = 0; // 0 both dx/dy
    int w = player->spriterect.w * player->spritescale;
    int h = player->spriterect.h * player->spritescale;

    // Update position
    if(game->keyboard[SDL_SCANCODE_UP]) player->dy -= player->speed;
    if(game->keyboard[SDL_SCANCODE_DOWN]) player->dy += player->speed;
    if(game->keyboard[SDL_SCANCODE_LEFT]) player->dx -= player->speed;
    if(game->keyboard[SDL_SCANCODE_RIGHT]) player->dx += player->speed;

    player->x += player->dx;
    player->y += player->dy;

    if(player->x <= 0) player->x = 0;
    if(player->x >= (SCREEN_WIDTH - w)){
        player->x = SCREEN_WIDTH - w;
    }
    if(player->y <= 0) player->y = 0;
    if(player->y >= (SCREEN_HEIGHT - h)) {
        player->y = SCREEN_HEIGHT - h;
    }

    if(game->keyboard[SDL_SCANCODE_SPACE] && !((player->flags & EF_COOLDOWN) == EF_COOLDOWN)) {
        proj = create_projectile(player, projrect);
        proj->flags |= EF_PLAYER;
        proj->dy = -1; // Projectile going up
        proj->y -= 25;
        proj->spritescale = 0.75;
        wsl_add_entity(game, proj); // Add projectile to list
        player->flags |= EF_COOLDOWN;
        player->cooldown = 25;
    }
}

void update_projectile(Entity *proj, WSL_App *game) {
    // Kill the projectile if it's out of bounds
    if((proj->x <= 0) || (proj->x >= SCREEN_WIDTH) || 
            (proj->y <= 0) || (proj->y >= SCREEN_HEIGHT)) {
        proj->flags &= ~EF_ALIVE;
    }

    // Update the projectiles position
    proj->x += proj->dx * proj->speed;
    proj->y += proj->dy * proj->speed;
}

void update_asteroid(Entity *ast, WSL_App *game) {
    // Kill the asteroid if it's out of bounds
    if((ast->x <= 0) || (ast->x >= SCREEN_WIDTH) || 
            (ast->y <= 0) || (ast->y >= SCREEN_HEIGHT)) {
        ast->flags &= ~EF_ALIVE;
    }

    if(!((ast->flags & EF_COOLDOWN) == EF_COOLDOWN)) {
        ast->angle += 5;
        ast->cooldown = 5;
        ast->flags |= EF_COOLDOWN;
    }
    ast->x += ast->dx * ast->speed;
    ast->y += ast->dy * ast->speed;
}

void spawn_asteroid(WSL_App *game) {
    Entity *ast = create_asteroid();
    int x = mt_rand(ast->spriterect.w, SCREEN_WIDTH - ast->spriterect.w);
    int y = ast->spriterect.h * ast->spritescale;
    ast->x = x;
    ast->y = y;
    ast->dy = 1;
    wsl_add_entity(game, ast); // Add asteroid to game list
    game->asteroidspawn = mt_rand(15,55); // Set timer to spawn a new asteroid
}
