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
    ufo->frame += 1;
    if(ufo->frame % 3 == 0) {
        ufo->angle += 15;
    }
    if(ufo->frame > 60) {
        // 60 frames per second, so every second check the following:
        ufo->frame = 0;
        if(mt_chance(15)) ai->mvleft = !ai->mvleft; //15% chance to randomly change direction
    }
    //Move the ufo to a new x,y point along the ufo's bezier curve
    //(advance the ufo's bzt, get the vec2f of the new point)
    //Change direction if we are near bzt == 0 or bzt == 1
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
    newpos = get_vec2f_on_bezier(ai->bzst, ai->bzmid, ai->bzend, ai->bzt);
    ufo->x = newpos.x;
    ufo->y = newpos.y;
}

void ufo_damage(Entity *ufo, WSL_App *game) {
    ufo->health -= 1;
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
