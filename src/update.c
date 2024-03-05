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
            //Entity is dead, call death function, remove it
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

void update_player(Entity *player, WSL_App *game) {
    Entity *proj = NULL;
    SDL_Rect projrect = {845,0,13,57};
    player->dx = player->dy = 0; // 0 both dx/dy
    int w = player->spriterect.w * player->spritescale;
    int h = player->spriterect.h * player->spritescale;

    // Update position
    if(game->keyboard[SDL_SCANCODE_UP]) player->dy -= player->speed;
    if(game->keyboard[SDL_SCANCODE_DOWN]) player->dy += player->speed;
    if(game->keyboard[SDL_SCANCODE_LEFT]) {
        // Angle the player sprite left if the player is moving left
        player->dx -= player->speed;
        player->angle = -10;
    }
    if(game->keyboard[SDL_SCANCODE_RIGHT]){
        // Angle the player sprite right if the player is moving right
        player->dx += player->speed;
        player->angle = 10;
    }
    if(!game->keyboard[SDL_SCANCODE_LEFT] &&
            !game->keyboard[SDL_SCANCODE_RIGHT]) {
        player->angle = 0;
    }

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

    // Fire lasers!
    if(game->keyboard[SDL_SCANCODE_SPACE] && !((player->flags & EF_COOLDOWN) == EF_COOLDOWN)) {
        proj = create_projectile(player, projrect);
        proj->flags |= EF_PLAYER;
        proj->dy = -1; // Projectile going up
        /*
        if(player->angle < 0) {
            // line pointing in the direction the ship is pointing - adjust the
            // horizontal movement of the projectile based on the direction the
            // player is facing. 0.98 is cosine of an angle slightly less than
            // 10 degrees. 10 degrees "looks" right on screen and was chosen for
            // no other reason.
            // The math is right but it seems weird being able to shoot left and
            // right on the game... leaving this here commented out so I don't
            // have this idea again.
            proj->dx = -0.9848078;
            proj->angle = -10;
        } else if (player->angle > 0) {
            proj->dx = 0.9848078;
            proj->angle = 10;
        }
        */
        proj->y -= 25;
        proj->spritescale = player->spritescale;
        wsl_add_entity(game, proj); // Add projectile to list
        player->flags |= EF_COOLDOWN; // Turn on cooldown flag
        player->cooldown = 25; // Start cooldown timer, entities should have a "firerate"
    }
}

void update_projectile(Entity *proj, WSL_App *game) {
    Entity *other = NULL;
    SDL_Rect otherbox;
    SDL_Rect hitbox = get_hitbox(proj);
    // Kill the projectile if it's out of bounds
    if((proj->x <= 0) || (proj->x >= SCREEN_WIDTH) || 
            (proj->y <= 0) || (proj->y >= SCREEN_HEIGHT)) {
        proj->flags &= ~EF_ALIVE;
    }

    // If the projectile hit something, kill both it and the projectile
    other = game->entities;
    while(other) {
        if(other == proj) {
            other = other->next;
            continue;
        }
        otherbox = get_hitbox(other);
        if(check_collision_rect(hitbox, otherbox)) {
            //CONTACT
            if(entity_is_player(proj) && entity_is_enemy (other)) {
                // Player projectile hitting enemy
                proj->flags &= ~EF_ALIVE;
                other->flags &= ~EF_ALIVE;
            }
        }
        other = other->next;
    }

    // Update the projectiles position
    proj->x += proj->dx * proj->speed;
    proj->y += proj->dy * proj->speed;
}

void update_asteroid(Entity *ast, WSL_App *game) {
    // Kill the asteroid if it's out of bounds
    if((ast->x <= 0) || (ast->x >= SCREEN_WIDTH) || 
            (ast->y >= SCREEN_HEIGHT)) {
        ast->flags &= ~EF_ALIVE;
    }

    if(!((ast->flags & EF_COOLDOWN) == EF_COOLDOWN)) {
        ast->angle += 5;
        ast->cooldown = 1;
        ast->flags |= EF_COOLDOWN;
    }
    ast->x += ast->dx * ast->speed;
    ast->y += ast->dy * ast->speed;
}
