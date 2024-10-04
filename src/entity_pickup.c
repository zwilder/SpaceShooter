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

/*****
 * Pickups
 *
 * Basically, pickups are only going to be able to "collide" with the player
 * ship, and when they "die" they will call a death function to do... something.
 * Increase fire rate, upgrade guns, boost shields, increase speed... etc.
 * From spritesheet.xml (I really should write something to load the xml file
 * and then have a lookup table to search for these by name... or something):
<SubTexture name="powerupBlue.png" x="696" y="329" width="34" height="33"/>
<SubTexture name="powerupBlue_bolt.png" x="539" y="989" width="34" height="33"/>
<SubTexture name="powerupBlue_shield.png" x="777" y="679" width="34" height="33"/>
<SubTexture name="powerupBlue_star.png" x="776" y="895" width="34" height="33"/>
<SubTexture name="powerupGreen.png" x="774" y="613" width="34" height="33"/>
<SubTexture name="powerupGreen_bolt.png" x="766" y="80" width="34" height="33"/>
<SubTexture name="powerupGreen_shield.png" x="776" y="862" width="34" height="33"/>
<SubTexture name="powerupGreen_star.png" x="651" y="490" width="34" height="33"/>
<SubTexture name="powerupRed.png" x="491" y="182" width="34" height="33"/>
<SubTexture name="powerupRed_bolt.png" x="775" y="646" width="34" height="33"/>
<SubTexture name="powerupRed_shield.png" x="776" y="928" width="34" height="33"/>
<SubTexture name="powerupRed_star.png" x="774" y="977" width="34" height="33"/>
<SubTexture name="powerupYellow.png" x="774" y="761" width="34" height="33"/>
<SubTexture name="powerupYellow_bolt.png" x="740" y="761" width="34" height="33"/>
<SubTexture name="powerupYellow_shield.png" x="482" y="325" width="34" height="33"/>
<SubTexture name="powerupYellow_star.png" x="607" y="857" width="34" height="33"/>
<SubTexture name="star_bronze.png" x="778" y="794" width="31" height="30"/>
<SubTexture name="star_gold.png" x="778" y="557" width="31" height="30"/>
<SubTexture name="star_silver.png" x="778" y="527" width="31" height="30"/>
<SubTexture name="things_bronze.png" x="778" y="495" width="32" height="32"/>
<SubTexture name="things_gold.png" x="777" y="411" width="32" height="32"/>
<SubTexture name="things_silver.png" x="777" y="353" width="32" height="32"/>
<SubTexture name="shield_bronze.png" x="797" y="143" width="30" height="30"/>
<SubTexture name="shield_gold.png" x="797" y="113" width="30" height="30"/>
<SubTexture name="shield_silver.png" x="778" y="824" width="30" height="30"/>
<SubTexture name="bold_silver.png" x="810" y="837" width="19" height="30"/>
<SubTexture name="bolt_bronze.png" x="810" y="467" width="19" height="30"/>
<SubTexture name="bolt_gold.png" x="809" y="437" width="19" height="30"/>
 *****/

Entity* create_pickup(SDL_Rect spriterect) {
    //This creates a generic "pickup" entity, and should be called by a "spawn"
    //function.
    Entity *pickup = create_entity(spriterect);
    // Pickups by default start at the middle/top of the screen
    pickup->x = SCREEN_WIDTH / 2;
    pickup->y = 0;
    // Slowly move downwards
    pickup->dx = 0;
    pickup->dy = 1;
    pickup->speed = 4;
    //pickup->angle = 45; //Maybe make them spin?

    pickup->spritescale = 1;
    pickup->flags |= (EF_ALIVE | EF_PICKUP);
    pickup->update = &update_pickup;
    pickup->render = &entity_render;

    return pickup;
}

void spawn_shield_pickup(int x, int y, WSL_App *game) {
//<SubTexture name="powerupRed_shield.png" x="776" y="928" width="34" height="33"/>
//<SubTexture name="powerupYellow_shield.png" x="482" y="325" width="34" height="33"/>
//<SubTexture name="powerupBlue_shield.png" x="777" y="679" width="34" height="33"/>
//<SubTexture name="powerupGreen_shield.png" x="776" y="862" width="34" height="33"/>
    SDL_Rect spriterect = {776, 928, 34, 33};
    Entity *pickup = create_pickup(spriterect);
    pickup->x = x;
    pickup->y = y;

    pickup->deathfunc = &shield_pickup_death;
    wsl_add_entity(game, pickup);
}

void update_pickup(Entity *pickup, WSL_App *game) {
    Entity *other = NULL;
    SDL_Rect otherbox;
    SDL_Rect hitbox = get_hitbox(pickup);
    // Kill the pickup if it's out of bounds
    if((pickup->x <= 0) || (pickup->x >= SCREEN_WIDTH) || 
            (pickup->y < 0) || (pickup->y >= SCREEN_HEIGHT)) {
        pickup->flags &= ~EF_ALIVE;
        pickup->flags |= EF_OOB;
    }

    // If the pickup hit something, kill it and call the death function 
    // (Boy that sounds dark)
    other = game->entities;
    while(other) {
        //Skip anything that isn't the player ship (since only the player ship
        //can hit the pickups)
        if(entity_is_player(other) && !entity_is_projectile(other)) {
            otherbox = get_hitbox(other);
            if(check_collision_rect(hitbox, otherbox)) {
                pickup->flags &= ~EF_ALIVE; //Pickup hit the player
            }
        }
        other = other->next;
    }
    
    // Update the pickup's position
    pickup->x += pickup->dx * pickup->speed;
    pickup->y += pickup->dy * pickup->speed;
}

void shield_pickup_death(Entity *pickup, WSL_App *game) {
    // Check to see if the shield pickup is OOB, and if not then it died from
    // contacting the player. Find the player and increase the health.

    Entity *player = NULL; 
    Entity *other = game->entities;
    if(!((pickup->flags & EF_OOB) == EF_OOB)) {
        while(other) {
            if(entity_is_player(other) && !entity_is_projectile(other)) {
                player = other;
                break;
            }
            other = other->next;
        }
        if(player) {
            player->health += 1;
            if(player->health > 8) {
                player->health = 8;
            }
            wsl_play_sound(game, SND_POWERUP0, CH_ANY);
        }
    }
}
