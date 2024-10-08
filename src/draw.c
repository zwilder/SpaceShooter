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

void draw_menu(WSL_App *game);
void draw_game(WSL_App *game);
void draw_scores(WSL_App *game);
void draw_gameover(WSL_App *game);

void draw(WSL_App *game) {
    switch(game->state) {
        case GS_MENU:
            draw_menu(game);
            break;
        case GS_SCORES:
            draw_scores(game);
            break;
        case GS_GAME:
        case GS_GAMEOVER:
        default:
            draw_game(game);
            break;
    }
}

void draw_menu(WSL_App *game) {
    int x = 0, y = 0;
    Entity *tmp = NULL, *player = NULL;
    SDL_Color hud_color = {242,242,242,255};
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
        if(!entity_is_player(tmp)) {
            tmp->render(tmp, game);
        } else if (entity_is_player(tmp) && !entity_is_projectile(tmp)) {
            player = tmp;
        }
        tmp = tmp->next;
    }

    // Show the "Menu" (Write some stuff on the screen)
    x = SCREEN_WIDTH / 2;
    y = SCREEN_HEIGHT / 2;
    wsl_ctext_render(game, hud_color, x - ((FONT_SIZE * 12) / 2),
            y - (FONT_SIZE * 5), "S P A C E S H O O T E R");
    wsl_ctext_render(game, hud_color, x - ((FONT_SIZE * 7) / 2),
            y - (FONT_SIZE * 2), "[N] ew Game");
    wsl_ctext_render(game, hud_color, x - ((FONT_SIZE * 7) / 2),
            y - (FONT_SIZE * 1), "[H] igh Scores");
    wsl_ctext_render(game, hud_color, x - ((FONT_SIZE * 7) / 2),
            y, "[Q] uit");

    // I made this!
    hud_color.a -= 200;
    wsl_ctext_render(game, hud_color, 10, SCREEN_HEIGHT - FONT_SIZE,
            "Zach Wilder, 2024");

    // Present
    SDL_RenderPresent(game->renderer);
}

void draw_game(WSL_App *game) {
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
        wsl_ctext_render(game, hud_color, 20,2, "Score: %d",game->score);
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
        /*
        wsl_ctext_render(game, hud_color, SCREEN_WIDTH - (FONT_SIZE * 12), 0,
                "Entities: %d", count_entities(game->entities));
        */
        if(player->health == 8) {
            wsl_ctext_render(game, hud_color, 20, SCREEN_HEIGHT - FONT_SIZE - 2,
                    "Shield Charge: MAXIMUM%%");
        } else {
            wsl_ctext_render(game, hud_color, 20, SCREEN_HEIGHT - FONT_SIZE - 2,
                    "Shield Charge: %d%% ",player->health * 25);
        }
    } else {
        wsl_ctext_render(game, hud_color,
                (SCREEN_WIDTH - (FONT_SIZE * 9)) / 2,
                (SCREEN_HEIGHT - FONT_SIZE) / 2,
                "GAME OVER");
    }

    // Present
    SDL_RenderPresent(game->renderer);
}
void draw_scores(WSL_App *game) {
    int x = 0, y = 0;
    int i,j;
    Entity *tmp = NULL, *player = NULL;
    SDL_Color hud_color = {242,242,242,255};
    SDL_Color yellow = {242,242,0,255};
    SDL_Color tmp_color;
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
        if(!entity_is_player(tmp)) {
            tmp->render(tmp, game);
        } else if (entity_is_player(tmp) && !entity_is_projectile(tmp)) {
            player = tmp;
        }
        tmp = tmp->next;
    }
    if(game->scores) {
        x = SCREEN_WIDTH / 2;
        y = SCREEN_HEIGHT / 2;
        y -= FONT_SIZE * NUM_HIGHSCORES;
        wsl_ctext_render(game, hud_color, 
                (SCREEN_WIDTH - (FONT_SIZE * 10)) / 2,
                y,
                "H I G H  S C O R E S");
        // Find the longest string in scores and then justify everything to the
        // left
        j = 0;
        for(i = 0; i < NUM_HIGHSCORES; i++) {
            if(game->scores[i].namelength > j) {
                j = game->scores[i].namelength;
            }
        }
        x = (SCREEN_WIDTH - (j * FONT_SIZE))/2;
        y += FONT_SIZE;
        for(i = 0; i < NUM_HIGHSCORES; i++) {
            y += FONT_SIZE * 2;
            if(game->scores[i].recent) {
                tmp_color = yellow;
            } else {
                tmp_color = hud_color;
            }
            wsl_ctext_render(game,tmp_color,x,y,
                    "%s",game->scores[i].name);
            wsl_ctext_render(game,tmp_color,
                    x + (j * FONT_SIZE), y,
                    "..... %d", game->scores[i].score);
        }
    }
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

