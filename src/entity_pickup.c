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
    SDL_Rect spriterect = {776, 928, 34, 33};
    Entity *pickup = create_pickup(spriterect);
    pickup->x = x;
    pickup->y = y;

    pickup->deathfunc = &shield_pickup_death;
    wsl_add_entity(game, pickup);
}

void spawn_points_pickup(int x, int y, WSL_App *game) {
//<SubTexture name="things_bronze.png" x="778" y="495" width="32" height="32"/>
//<SubTexture name="things_gold.png" x="777" y="411" width="32" height="32"/>
//<SubTexture name="things_silver.png" x="777" y="353" width="32" height="32"/>
    SDL_Rect spriterect; 
    int type = mt_rand(1,3);
    spriterect.w = 32;
    spriterect.h = 32;
    switch(type) {
        case 1:
            spriterect.x = 778;
            spriterect.y = 495;
            break;
        case 2:
            spriterect.x = 777;
            spriterect.y = 411;
            break;
        case 3:
            spriterect.x = 777;
            spriterect.y = 353;
            break;
        default:
            break;
    }
    Entity *pickup = create_pickup(spriterect);
    pickup->x = x;
    pickup->y = y;
    switch(type) {
        case 1:
            pickup->deathfunc = &small_points_pickup_death;
            break;
        case 2:
            pickup->deathfunc = &med_points_pickup_death;
            break;
        case 3:
        default:
            pickup->deathfunc = &lg_points_pickup_death;
            break;
    }
    wsl_add_entity(game, pickup);

}

void spawn_random_pickup(Entity *from, WSL_App *game, int chance) {
    if(mt_chance(chance)) {
        //choose a random pickup to spawn here
        switch(mt_rand(1,2)) {
            case 1:
                spawn_shield_pickup(from->x,from->y, game); 
                break;
            case 2:
            default:
                spawn_points_pickup(from->x, from->y, game);
                break;
        }
    }
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
    Entity *player = find_player(game);
    if(!entity_is_oob(pickup) && player) {
        player->health += 1;
        if(player->health > 8) {
            player->health = 8;
        }
        wsl_play_sound(game, SND_POWERUP0, CH_ANY);
    }
}

void points_pickup_death(Entity *pickup, WSL_App *game, int points) {
    Entity *player = find_player(game);
    if(!entity_is_oob(pickup) && player) {
        game->score += points;
        wsl_play_sound(game, SND_POWERUP0, CH_ANY);
    }
}

void small_points_pickup_death(Entity *pickup, WSL_App *game) {
    points_pickup_death(pickup, game, 50);
}

void med_points_pickup_death(Entity *pickup, WSL_App *game) {
    points_pickup_death(pickup, game, 100);
}

void lg_points_pickup_death(Entity *pickup, WSL_App *game) {
    points_pickup_death(pickup, game, 150);
}
