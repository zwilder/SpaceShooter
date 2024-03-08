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

Entity* create_player(SDL_Rect spriterect) {
    /*
     * Player entity creation function. Sets the player update function, generic
     * entity rendering function, and (eventually) will set the player death
     * function. The SDL_Rect is passed in with the ambition of someday having a
     * "choose your ship:" screen or some other player creation screen.
     */
    Entity *player = create_entity(spriterect);
    player->flags = EF_ALIVE | EF_PLAYER;
    player->update = &update_player;
    player->render = &entity_render;
    player->speed = 8;
    player->spritescale = 0.75;
    return player;
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

    /*
    if(player->particletimer <= 0) {
        particle = create_particle_test(player, game);
        particle->y += player->spriterect.h * player->spritescale;
        //particle->x = player->x;
        particle->x += (player->spriterect.w * player->spritescale) / 2;
        //particle->rgba[1] = 0;
        //particle->rgba[2] = 0;
        particle->rgba[3] = 125;
        wsl_add_entity(game, particle);
        player->particletimer = 5;
    } else {
        player->particletimer -= 1;
    }
    */
}
