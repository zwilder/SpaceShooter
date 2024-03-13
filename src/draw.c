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
    Entity *tmp = NULL, *player = NULL;
    int x = 0, y = 0;
    SDL_Color hud_color = {242,242,242};
    //SDL_Rect hitbox;

    // Clear the screen
    SDL_SetRenderDrawColor(game->renderer, 0xFF, 0x00, 0x00, 0xFF);
    SDL_RenderClear(game->renderer);

    // Draw the background, tiled across the screen, scrolling along the offset
    for(y = -1 * game->bg->h; y < SCREEN_HEIGHT; y += game->bg->h) {
        for(x = 0; x < SCREEN_WIDTH; x += game->bg->w) {
            //wsl_texture_render(game->bg,x,y);
            wsl_texture_render(game->bg,x,y+game->bgoffset);
        }
    }

    // Render the entities (and find the player)
    tmp = game->entities;
    while(tmp) {
        tmp->render(tmp, game);
        //hitbox = get_hitbox(tmp);
        //SDL_RenderDrawRect(game->renderer, &hitbox);
        if(entity_is_player(tmp) && !entity_is_projectile(tmp)) {
            player = tmp;
        }
        tmp = tmp->next;
    }

    // Render the HUD
    if(player) {
        wsl_texture_load_text(game, game->hud_text, hud_color, 
                "Points: 0");
        wsl_texture_render(game->hud_text, 10,0);
        switch(player->health) {
            case 1:
                hud_color.g -= 50;
                hud_color.b -= 50;
                //fall thru (less life is more red)
            case 2:
                hud_color.g -= 50;
                hud_color.b -= 50;
                break;
            case 3:
                hud_color.r -= 50;
                hud_color.b -= 50;
                break;
            default: break;
        }
        wsl_texture_load_text(game, game->hud_text, hud_color, 
                "Hull Integrity: %d%%", player->health * 25);
        wsl_texture_render(game->hud_text, 10,26);
    } else {
        wsl_texture_load_text(game, game->hud_text, hud_color,
                "GAME OVER");
        wsl_texture_render(game->hud_text, 
                (SCREEN_WIDTH - game->hud_text->w) / 2, 
                (SCREEN_HEIGHT - game->hud_text->h) / 2);
    }

    // Present
    SDL_RenderPresent(game->renderer);
}
