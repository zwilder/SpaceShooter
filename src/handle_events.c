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

void handle_keydown_menu(SDL_KeyboardEvent *event, WSL_App *game);
void handle_keydown_game(SDL_KeyboardEvent *event, WSL_App *game);
void handle_keydown_scores(SDL_KeyboardEvent *event, WSL_App *game);
void handle_keydown_gameover(SDL_KeyboardEvent *event, WSL_App *game);

void handle_events(WSL_App *game) {
    SDL_Event e;
    while(SDL_PollEvent(&e) != 0) {
        switch(e.type) {
            case SDL_QUIT:
                game->running = false;
                break;
            case SDL_KEYDOWN:
                handle_keydown(&e.key, game);
                break;
            case SDL_KEYUP:
                handle_keyup(&e.key, game);
                break;
            default:
                break;
        }
    }
}

void handle_keydown(SDL_KeyboardEvent *event, WSL_App *game) {
    switch(game->state) {
        case GS_MENU:
            handle_keydown_menu(event,game);
            break;
        case GS_SCORES:
            break;
        case GS_GAME:
        case GS_GAMEOVER:
        default:
            handle_keydown_game(event, game);
            break;
    }
}

void handle_keydown_menu(SDL_KeyboardEvent *event, WSL_App *game) {
    int i,x,y;
    switch(event->keysym.sym) {
        case SDLK_SPACE:
            for(i = 0; i < mt_rand(5,10); i++) {
                x = mt_rand(0,SCREEN_WIDTH);
                y = mt_rand(0,SCREEN_HEIGHT);
                spawn_random_color_explosion(x,y, game);
            }
            //spawn_bliptxt(10,10,game,"TEST TEXT!",60,255,0,0,250);
            break;
        case SDLK_n:
            game->state = GS_NEW;
            break;
        case SDLK_h:
            game->state = GS_SCORES;
            break;
        case SDLK_q:
        case SDLK_ESCAPE:
            game->running = false;
            break;
        default: break;
    }
}

void handle_keydown_game(SDL_KeyboardEvent *event, WSL_App *game) {
    if((event->repeat == 0) && (event->keysym.scancode < MAX_KEYBOARD_KEYS)) {
        game->keyboard[event->keysym.scancode] = true;
    }
    switch(event->keysym.sym) {
        case SDLK_q:
        case SDLK_ESCAPE:
            game->running = false;
            break;
        default: break;
    }
}

void handle_keyup(SDL_KeyboardEvent *event, WSL_App *game) {
    if((event->repeat == 0) && (event->keysym.scancode < MAX_KEYBOARD_KEYS)) {
        game->keyboard[event->keysym.scancode] = false;
    }
}
