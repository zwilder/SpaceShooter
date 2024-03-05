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
#include <sys/time.h>

long current_ms(void) {
    // Helper function to return the current time in ms
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return ((tp.tv_sec * 1000) + (tp.tv_usec / 1000));
}

int main(int argc, char **argv) {
    long lag = 0, current = 0, elapsed = 0;
    long prev = current_ms();
    long msperframe = 16; // 16ms = ~60fps, 33ms = ~30fps
    WSL_App *game = wsl_init_sdl(); // Start SDL

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

    /* Basic game loop, straight outta Game Programming Patterns */
    while(game->running) {
        current = current_ms();
        elapsed = current - prev;
        prev = current;
        lag += elapsed;

        //Handle events
        handle_events(game);

        //Update
        while(lag >= msperframe) {
            lag -= msperframe;
            update(game);
        }

        //Draw
        draw(game);
    }

    wsl_cleanup_sdl(game);
    return 0;
}
