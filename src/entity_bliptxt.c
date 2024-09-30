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

void spawn_bliptxt(int x, int y, WSL_App *game, char *txt, int life, int speed,
        uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    /*
     * Creates a short lived entity that is displayed as the string "txt", that
     * flashes in/out in a super cool spaceship sorta way then dissappears.
     */
    SDL_Rect spriterect = {0,0,0,0};
    Entity *blip = create_entity(spriterect);
    blip->flags = EF_ALIVE | EF_BLIP;
    blip->update = &update_bliptxt;
    blip->render = &bliptxt_render;
    blip->rgba[0] = r;
    blip->rgba[1] = g;
    blip->rgba[2] = b;
    blip->rgba[3] = a;
    blip->cooldown = life; // How long the blip lasts before disappearing
    blip->speed = speed; // The smaller this number is, the less blinky
    blip->x = x;
    blip->y = y;
    blip->txt = malloc(strlen(txt) * sizeof(char) + 1);
    strncpy(blip->txt, txt, strlen(txt) + 1);

    wsl_add_entity(game, blip);
}

void update_bliptxt(Entity *blip, WSL_App *game) {
    // Modulate the blip's alpha, so it flashes when rendered
    // Check to see if the blip should be killed (timer ran out)
    int a = blip->speed;
    if(mt_bool()) {
        a *= -1;
    }

    if(blip->cooldown) {
        blip->rgba[3] += a; 
    } else {
        blip->flags &= ~EF_ALIVE;
    }
}

void bliptxt_render(Entity *blip, WSL_App *game) {
    SDL_Color hud_color = {blip->rgba[0],blip->rgba[1],
        blip->rgba[2],blip->rgba[3]};
    int x = blip->x;
    int y = blip->y;
    wsl_ctext_render(game, hud_color,x,y,blip->txt);
}
