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

#ifndef SPACESHOOTER_H
#define SPACESHOOTER_H

/*****
 * System
 *****/
#include <stdio.h>
#include <stdbool.h>

/*****
 * SDL2
 *****/
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

/*****
 * Toolbox
 *****/
#include <mt19937.h>

/*****
 * Project
 *****/
#include <defs.h>
#include <entity.h>
#include <wsl_sdl.h>
#include <handle_events.h>
#include <update.h>
#include <draw.h>

#endif //SPACESHOOTER_H
