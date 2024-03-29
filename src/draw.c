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
    SDL_Color hud_color = {242,242,242,255};
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
        wsl_texture_render(game->hud_text, 20,2);
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
        wsl_texture_render(game->hud_text,20, SCREEN_HEIGHT - FONT_SIZE - 2);
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
/*
 * TODO: This function was giving me grief, and I was getting bored with trying
 * to make the UI pretty. Fix this later.
 *
void draw_glass_panel(WSL_App *game, SDL_Rect grid) {
	//<SubTexture name="glassPanel.png" x="0" y="0" width="100" height="100"/>
    //<SubTexture name="metalPanel.png" x="200" y="200" width="100" height="100"/>
    //Each panel texture in the uipackSpace_sheet.xml is the same size EXCEPT
    //the glassPanel_tab sprite - so if we just add each x/y coordinate of the
    //other textures to the basic glass panel... we can use the different
    //sprites
    // Break the panel texture into a 9x9 grid
    int x = grid.x;
    int y = grid.y;
    int w = grid.w;
    int h = grid.h;
    int i = 0, j = 0;
    int sx = 0; // Spritesheet x coord
    int sy = 0; // Spritesheet y coord
    SDL_Rect panelTL = {0+sx,0+sy,34,34};
    SDL_Rect panelTM = {34+sx,0+sy,33,34};
    SDL_Rect panelTR = {67+sx,0+sy,33,34};

    SDL_Rect panelML = {0+sx,34+sy,34,33};
    SDL_Rect panelMM = {34+sx,34+sy,33,33};
    SDL_Rect panelMR = {67+sx,34+sy,33,33};

    SDL_Rect panelBL = {0+sx,67+sy,34,33};
    SDL_Rect panelBM = {34+sx,67+sy,33,33};
    SDL_Rect panelBR = {67+sx,67+sy,33,33};

    SDL_Rect renderquad = {x,y,panelTL.w,panelTL.h};

    //w and h are "units" of the panel

    //Starting at x,y draw the TL panel
    SDL_RenderCopy(game->renderer, game->ui_spritesheet->tex, &panelTL, &renderquad);

    //draw TM at x+(i*TL.w),y while i < w
    renderquad.w = panelTM.w;
    renderquad.h = panelTM.h;
    renderquad.x += 1;
    for(i = 1; i < w; i++) {
        renderquad.x += panelTM.w;
        SDL_RenderCopy(game->renderer, game->ui_spritesheet->tex, &panelTM, &renderquad);
    }
    //draw TR at x+(w*TL.w),y
    renderquad.w = panelTR.w;
    renderquad.h = panelTR.h;
    renderquad.x += panelTM.w;
    SDL_RenderCopy(game->renderer,game->ui_spritesheet->tex, &panelTR, &renderquad);

}
*/

