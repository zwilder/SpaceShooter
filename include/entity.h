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

#ifndef ENTITY_H
#define ENTITY_H

#include <wsl_sdl.h>

typedef enum {
    EF_NONE         = 1 << 0,
    EF_ALIVE        = 1 << 1,
    EF_PLAYER       = 1 << 2,
    EF_ENEMY        = 1 << 3,
    EF_COOLDOWN     = 1 << 4,
    EF_PROJECTILE   = 1 << 5
} EntityFlags;

typedef struct Entity Entity;

struct Entity {
    int x; // Will probably replace with a Vec2i
    int y;
    int dx;
    int dy;
    int speed; // How fast the entity is
    int cooldown;
    int flags;
    SDL_Rect spriterect; // Rect of the player sprite, spritesheet.xml
    Entity *next; // Entity is a linked list node, WSL_App contains the head
    Entity *prev;
    void (*update)(Entity*, WSL_App*); // Entity update function
    void (*render)(Entity*, WSL_App*); // Entity render function
};

Entity* create_entity(SDL_Rect spriterect);
void destroy_entity(Entity *entity);
int count_entities(Entity *entity);
Entity* create_player(SDL_Rect spriterect);
Entity* create_projectile(Entity *from, SDL_Rect spriterect);

void update_player(Entity *player, WSL_App *game);
void update_projectile(Entity *proj, WSL_App *game);

void entity_render(Entity *entity, WSL_App *game);

#endif //ENTITY_H
