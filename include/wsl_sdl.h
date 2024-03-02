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

typedef struct {
    SDL_Texture *tex;
    SDL_Renderer *renderer; // Maybe unnecessary
    int w;
    int h;
} WSL_Texture;

/* Just an idea
typedef struct {
    WSL_Texture *t; // Texture of the spritesheet
    SDL_Rect *clips; // Array of rects for each sprite
    char **clipnames; // Array of string names for each sprite
} WSL_Spritesheet;
*/

typedef struct {
    SDL_Window *window;
    SDL_Surface *screen_surface;
    SDL_Renderer *renderer;
    WSL_Texture *spritesheet;
    bool running; // Will likely be replaced with bitflags tlater
} WSL_SDL_App;

/*****
 * WSL_SDL_App
 *****/
WSL_SDL_App* wsl_init_sdl(void);
void wsl_cleanup_sdl(WSL_SDL_App *app);
bool wsl_load_media(WSL_SDL_App *app);

/*****
 * WSL_Texture
 *****/
WSL_Texture* create_wsl_texture(SDL_Renderer *renderer);
void destroy_wsl_texture(WSL_Texture *t);
bool wsl_texture_load(WSL_Texture *t, char *path);

#endif //WSL_SDL_H
