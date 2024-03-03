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
    int speed = 8;

    SDL_Rect playerrect = {211, 941, 99 ,75};

    Entity *player = create_entity(playerrect);
    player->x = (SCREEN_WIDTH / 2) - playerrect.w;
    player->y = (SCREEN_HEIGHT) - playerrect.h;

    if(!game) {
        printf("Failed to create WSL_SDL_App!\n");
        return 1;
    }

    while(game->running) {
        //Handle events
        handle_events(game);

        //Update
        
        if(game->up) player->y -= speed;
        if(game->down) player->y += speed;
        if(game->left) player->x -= speed;
        if(game->right) player->x += speed;

        if(player->x <= 0) player->x = 0;
        if(player->x >= (SCREEN_WIDTH - player->spriterect.w)){
            player->x = SCREEN_WIDTH - player->spriterect.w;
        }
        if(player->y <= 0) player->y = 0;
        if(player->y >= (SCREEN_HEIGHT - player->spriterect.h)) {
            player->y = SCREEN_HEIGHT - player->spriterect.h;
        }

        //Draw
        // Clear the screen
        SDL_SetRenderDrawColor(game->renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(game->renderer);

        // Draw player sprite
        wsl_texture_render_rect(game->spritesheet, player->x, player->y, 
                &player->spriterect);

        // Present
        SDL_RenderPresent(game->renderer);
    }

    destroy_entity(player);
    wsl_cleanup_sdl(game);
    return 0;
}
