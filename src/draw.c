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

void draw(WSL_App *game) {
    Entity *tmp = NULL;

    // Clear the screen
    SDL_SetRenderDrawColor(game->renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(game->renderer);

    // Render the entities
    tmp = game->entities;
    while(tmp) {
        tmp->render(tmp, game);
        tmp = tmp->next;
    }

    // Present
    SDL_RenderPresent(game->renderer);
}
