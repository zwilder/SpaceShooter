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
     * Player entity creation function. Sets the player update function, player
     * entity rendering function, damage function, and the player death
     * function. The SDL_Rect is passed in with the ambition of someday having a
     * "choose your ship:" screen or some other player creation screen.
     */
    Entity *player = create_entity(spriterect);
    player->flags = EF_ALIVE | EF_PLAYER;
    player->update = &update_player;
    player->render = &player_render;
    player->take_damage = &player_damage;
    player->deathfunc = &firework_death;
    player->speed = 8;
    player->spritescale = 0.75;
    player->health = 4;
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
        wsl_play_sound(game, SND_PLAYER_FIRE, CH_PLAYER);
    }
    if((player->flags & EF_INV) == EF_INV) {
        player->frame -= 1;
        player->cooldown = 25;
        if(player->frame % 2 == 0) {
            player->rgba[3] = 150;
        } else {
            player->rgba[3] = 25;
        }
        if(player->frame <= 0) {
            player->flags &= ~EF_INV;
            player->flags &= ~EF_COOLDOWN;
            player->cooldown = 0;
            player->rgba[3] = 255;
        }
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

void player_damage(Entity *player, WSL_App *game) {
    // I should make the player "invulnerable" after taking damage for a couple
    // seconds..
    if(!((player->flags & EF_INV) == EF_INV)) {
        player->health -= 1;
        if(player->health <= 0) {
            player->flags &= ~EF_ALIVE;
            wsl_play_sound(game, mt_rand(SND_EXPLODE0, SND_EXPLODE4), CH_ANY);
        }
        player->flags |= EF_INV | EF_COOLDOWN;
        player->frame = 120; //60fps, 120 is 2 seconds
        player->cooldown = 25; // Can't shoot while "invulnerable"
        player->rgba[3] = 25; // Show the ship damage sprites 
    }
}

void player_render(Entity *player, WSL_App *game) {
    /*
     * Render the player entity and the "damage" graphic directly under the
     * player in the same space, so that when the player takes damage the ship
     * "looks" damaged.
     */
    SDL_Rect damagerect = {0,0,0,0};
    SDL_Rect dmgquad = {0,0,0,0};
    switch(player->health) {
        case(4):
        case(3):
            damagerect.x = 112;
            damagerect.y = 941;
            damagerect.w = 99;
            damagerect.h = 75;
            break;
        case(2):
            damagerect.x = 247;
            damagerect.y = 234;
            damagerect.w = 99;
            damagerect.h = 75;
            break;
        case(1):
            damagerect.x = 247;
            damagerect.y = 159;
            damagerect.w = 99;
            damagerect.h = 75;
            break;
        default:
            break;
    }
    if(damagerect.x) {
        //render the damage rect under the player, so it's visable when the
        //player is transparent (after they take damage)
        SDL_SetTextureColorMod(game->spritesheet->tex, 125,125,125);
        SDL_SetTextureAlphaMod(game->spritesheet->tex, 255);
        dmgquad.x = player->x;
        dmgquad.y = player->y;
        dmgquad.w = damagerect.w * player->spritescale;
        dmgquad.h = damagerect.h * player->spritescale;
        SDL_RenderCopyEx(game->renderer, game->spritesheet->tex,
                &damagerect, &dmgquad,
                player->angle, NULL, SDL_FLIP_NONE);
    }
    entity_render(player, game);
}
