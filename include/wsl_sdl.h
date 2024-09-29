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

#ifndef WSL_SDL_H
#define WSL_SDL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

typedef struct Entity Entity;

typedef enum {
    GS_MENU,
    GS_NEW,
    GS_GAME,
    GS_SCORES,
    GS_GAMEOVER
} GameStates;

typedef struct {
    SDL_Texture *tex;
    SDL_Renderer *renderer; // Maybe unnecessary
    int w;
    int h;
} WSL_Texture;

typedef struct {
    SDL_Window *window; // The SDL Window
    SDL_Surface *screen_surface;
    SDL_Renderer *renderer; // The SDL Renderer
    TTF_Font *font; // SDL Font
    WSL_Texture *spritesheet; // Spritesheet with all the sprites
    WSL_Texture *ui_spritesheet; // Spritesheet with all the UI textures
    WSL_Texture *bg; // Background texture, will be an array eventually?
    WSL_Texture *hud_text; // Heads up display text
    int bgoffset; // Scrollin' background offset
    Entity *entities; // Linked list of all the entities
    bool keyboard[MAX_KEYBOARD_KEYS]; // Keypress "flags" for all keys
    bool running; // Will likely be replaced with bitflags tlater
    int state; // Current game state
    int asteroidspawn; // Asteroid spawn timer
    int score; // The current player score
} WSL_App;

/*****
 * WSL_App
 *****/
WSL_App* wsl_init_sdl(void);
void wsl_cleanup_sdl(WSL_App *app);
bool wsl_load_media(WSL_App *app);
void wsl_add_entity(WSL_App *app, Entity *entity);
Entity* wsl_remove_entity(WSL_App *app, Entity *entity);
void wsl_destroy_entity(WSL_App *app, Entity *entity);

/*****
 * WSL_Texture
 *****/
WSL_Texture* create_wsl_texture(SDL_Renderer *renderer);
void destroy_wsl_texture(WSL_Texture *t);
bool wsl_texture_load(WSL_Texture *t, char *path);
bool wsl_texture_load_text(WSL_App *app, WSL_Texture *t, 
        SDL_Color color, char *fstr, ...);
void wsl_texture_render(WSL_Texture *t, int x, int y);
void wsl_texture_render_rect(WSL_Texture *t, int x, int y, SDL_Rect *rect);
void wsl_texture_render_rect_scaled(WSL_Texture *t, int x, int y, 
        SDL_Rect *rect, float scale);

#endif //WSL_SDL_H
