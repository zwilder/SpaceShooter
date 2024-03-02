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
    WSL_SDL_App *game = wsl_init_sdl();
    SDL_Event e;

    if(!game) {
        printf("Failed to create WSL_SDL_App!\n");
        return 1;
    }

    while(game->running) {
        //Update
        
        //Handle events
        while(SDL_PollEvent(&e) != 0) {
            if(e.type == SDL_QUIT) {
                game->running = false;
            } else if(e.type == SDL_KEYDOWN) {
                switch(e.key.keysym.sym) {
                    case SDLK_q:
                    case SDLK_ESCAPE:
                        game->running = false;
                    default: break;
                }

            }
        }

        //Draw
        // Clear the screen
        SDL_SetRenderDrawColor(game->renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(game->renderer);

        // Present
        SDL_RenderPresent(game->renderer);
    }

    wsl_cleanup_sdl(game);
    return 0;
}
