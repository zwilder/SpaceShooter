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

int main(int argc, char **argv) {
    WSL_App *game = wsl_init_sdl();
    init_genrand(time(NULL)); // Seed the pnrg

    // Temporary player creation
    SDL_Rect playerrect = {211, 941, 99 ,75};
    Entity *player = create_player(playerrect);
    player->x = (SCREEN_WIDTH / 2) - playerrect.w;
    player->y = (SCREEN_HEIGHT) - playerrect.h;
    wsl_add_entity(game, player);

    if(!game) {
        printf("Failed to create WSL_App!\n");
        return 1;
    }

    while(game->running) {
        //Handle events
        handle_events(game);

        //Update
        update(game);

        //Draw
        draw(game);

        //Delay to set framerate/game speed?
        //I took this from the Parallel Realities SDL Tutorials
        SDL_Delay(16);
    }

    wsl_cleanup_sdl(game);
    return 0;
}
