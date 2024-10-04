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

#ifndef DEFS_H
#define DEFS_H

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

#define MAX_KEYBOARD_KEYS 350
#define FONT_SIZE 20

#define MAX_SND_CHANNELS 8

enum {
    CH_ANY = -1,
    CH_PLAYER,
    CH_ALIEN,
    CH_IMPACT
};

enum {
    SND_PLAYER_FIRE,
    SND_ALIEN_FIRE,
    SND_ALIEN_SPAWN,
    SND_EXPLODE0,
    SND_EXPLODE1,
    SND_EXPLODE2,
    SND_EXPLODE3,
    SND_EXPLODE4,
    SND_IMPACT0,
    SND_IMPACT1,
    SND_IMPACT2,
    SND_IMPACT3,
    SND_IMPACT4,
    SND_POWERUP0,
    SND_MAX
};

#endif //DEFS_H
