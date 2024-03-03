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

#ifndef HANDLE_EVENTS_H
#define HANDLE_EVENTS_H

#include <wsl_sdl.h>

void handle_events(WSL_App *game);
void handle_keydown(SDL_KeyboardEvent *event, WSL_App *game);
void handle_keyup(SDL_KeyboardEvent *event, WSL_App *game);

#endif //HANDLE_EVENTS_H
