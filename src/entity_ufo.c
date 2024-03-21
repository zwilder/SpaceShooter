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

Entity* create_ufo(SDL_Rect spriterect) {
    Entity *ufo = create_entity(spriterect);
    ufo->flags = EF_ENEMY;
    ufo->angle = 15;
    ufo->speed = 8;
    ufo->health = 2;
    ufo->ai = malloc(sizeof(EntityAI));
    float xoff = spriterect.w / 2;
    //Start point of curve should be either the top left or top right of screen
    ufo->ai->bzst.y = 0;
    ufo->ai->bzst.x = (mt_bool() ? (-1 * xoff) : (SCREEN_WIDTH + xoff));

    // Move left if we start on the right, right if we start on the left
    ufo->ai->mvleft = (ufo->ai->bzst.x ? true : false);

    //Mid point of curve, temporarily (or by default?) the middle of the screen
    ufo->ai->bzmid.x = SCREEN_WIDTH / 2;
    ufo->ai->bzmid.y = SCREEN_HEIGHT / 2;

    //End point of curve, somewhere on the opposite side of the screen from the
    //start point
    if(ufo->ai->bzst.x >= SCREEN_WIDTH) {
        // Starting on the right, end on left
        ufo->ai->bzend.x = -1 * xoff;
    } else {
        // Starting on the left, end on right
        ufo->ai->bzend.x = SCREEN_WIDTH + xoff;
    }
    //ufo->ai->bzend.x = ufo->ai->bzst.x ? (-10) : (SCREEN_WIDTH - 10);
    ufo->ai->bzend.y = (SCREEN_HEIGHT / 2)*(float)genrand_real1();

    //Interpolation point starts at 0
    ufo->ai->bzt = 0;

    ufo->update = &ufo_update;
    ufo->render = &entity_render;
    ufo->take_damage = &ufo_damage;
    ufo->deathfunc = &ufo_death;

    return ufo;
}

void spawn_ufo(WSL_App *game, Entity *from) {
	//<SubTexture name="ufoBlue.png" x="444" y="91" width="91" height="91"/>
	//<SubTexture name="ufoGreen.png" x="434" y="234" width="91" height="91"/>
	//<SubTexture name="ufoRed.png" x="444" y="0" width="91" height="91"/>
	//<SubTexture name="ufoYellow.png" x="505" y="898" width="91" height="91"/>
    SDL_Rect spriterect = {505,898,91,91};
    switch(mt_rand(1,4)) {
        case 1:
            spriterect.x = 444;
            spriterect.y = 91;
            break;
        case 2:
            spriterect.x = 434;
            spriterect.y = 234;
            break;
        case 3:
            spriterect.x = 444;
            spriterect.y = 0;
            break;
        default:
            spriterect.x = 505;
            spriterect.y = 898;
            break;
    }
    spriterect.w = 91;
    spriterect.h = 91;
    Entity *ufo = create_ufo(spriterect);
    ufo->spritescale = 0.75;
    ufo->flags |= EF_ALIVE;
    if(from) {
        ufo->x = from->x;
        ufo->y = from->y;
        ufo->ai->bzst.x = from->x;
        ufo->ai->bzst.y = from->y;
        ufo->ai->mvleft = mt_bool();
    } else {
        ufo->x = ufo->ai->bzst.x;
        ufo->y = ufo->ai->bzst.y;
    }
    wsl_add_entity(game, ufo);
}

void ufo_update(Entity *ufo, WSL_App *game) {
    Vec2f newpos = {};
    EntityAI *ai = ufo->ai;
    Entity *proj = NULL;
	//<SubTexture name="laserRed06.png" x="843" y="903" width="13" height="37"/>
    SDL_Rect projrect = {843,903,13,37};
    ufo->frame += 1; //Update the ufo's internal clock
    if(ufo->frame % 3 == 0) {
        // Every third frame rotate the ship a bit. 
        ufo->angle += 15;
    }
    if(ufo->frame > 60) {
        // 60 frames per second, so every second check the following:
        ufo->frame = 0;
        if(mt_chance(15)) ai->mvleft = !ai->mvleft; //15% chance to randomly change direction
        if((ufo->flags & EF_INV) == EF_INV) {
            //Turn off the invulnerable flag
            ufo->flags &= ~EF_INV;
            ufo->rgba[3] = 255;
            ufo->speed = 8;
        }
    }
    //Flash sprite if invulnerable
    if((ufo->flags & EF_INV) == EF_INV) {
        if(ufo->frame % 2 == 0) {
            ufo->rgba[3] = 150;
        } else {
            ufo->rgba[3] = 25;
        }
    }

    //Spawn smoke particles if damaged
    if(ufo->health == 1) {
        //Placeholder: Haven't attempted smoke particles yet
        //(Just didn't want to forget this idea. TODO
    }

    //Move the ufo to a new x,y point along the ufo's bezier curve
    //(advance the ufo's bzt, get the vec2f of the new point)
    //Change direction if we are near bzt == 0 or bzt == 1
    /*
     * Note for future self, a cubic curve made with start.x,start.y, ctA.x,start.y,
     * start.x,ctB.y, and ctA.x,ctB.x makes a nice backwards "S" on the screen
     */
    if(ai->mvleft) {
        ai->bzt -= ((float)ufo->speed / 1000);
    } else {
        ai->bzt += ((float)ufo->speed / 1000);
    }
    if(ai->bzt <= 0) {
        ai->bzt = 0;
        if(mt_bool()) {
            // Sometimes, change the end point when you reach the start
            ai->bzend.y = (SCREEN_HEIGHT)*(float)genrand_real1();
        }
        ai->mvleft = false;
    }
    if(ai->bzt >= 1) {
        ai->bzt = 1;
        ai->mvleft = true;
        if(mt_bool()) {
            // Sometimes, change the end point when you reach the start
            ai->bzst.y = (SCREEN_HEIGHT)*(float)genrand_real1();
        }
    }
    newpos = get_vec2f_bezier_opt(ai->bzst, ai->bzmid, ai->bzend, ai->bzt);
    ufo->x = newpos.x;
    ufo->y = newpos.y;

    // Fire lasers!
    if(!ufo->cooldown && (!((ufo->flags & EF_INV) == EF_INV))) {
        proj = create_projectile(ufo, projrect);
        proj->flags |= EF_ENEMY;
        proj->dy = 1; // Projectile going down
        proj->angle = 180;
        proj->spritescale = ufo->spritescale;
        wsl_add_entity(game, proj); // Add projectile to list
        ufo->flags |= EF_COOLDOWN; // Turn on cooldown flag
        ufo->cooldown = 60; // Start cooldown timer, entities should have a "firerate"
    }
}

void ufo_damage(Entity *ufo, WSL_App *game) {
    if((ufo->flags & EF_INV) == EF_INV) return;
    ufo->health -= 1;
    ufo->flags |= EF_INV;
    ufo->speed = 24; // ZIP AWAY
    ufo->frame = 0; // Reset frame, UFO is invulnerable for a second after hit
    if(ufo->health <= 0) {
        ufo->flags &= ~EF_ALIVE;
    }
}

void ufo_death(Entity *ufo, WSL_App *game) {
    // Eventually, will write a particle explosion that breaks the ufo's
    // spriterect into a 3x3 grid, with each cell acting as a "particle" in the
    // explosion
    //
    //firework_death(ufo, game);
    spawn_random_color_explosion(ufo->x,ufo->y,game);
}
