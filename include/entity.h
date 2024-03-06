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
    EF_PROJECTILE   = 1 << 5,
    EF_OOB          = 1 << 6
} EntityFlags;

typedef struct Entity Entity;

struct Entity {
    float x; // Will probably replace with a float, or Vec2f even 
    float y; // Entity's x,y coordinates on the screen
    float dx; // The change (delta) in the x,y coordinates
    float dy;
    double angle; // Angle the sprite is rendered at
    int speed; // How fast the entity is
    int cooldown; // Action cooldown timer
    int particletimer; // Particle spawn timer
    int frame; // Animation frame timer
    int flags; // EntityFlags
    uint8_t rgba[4];
    SDL_Rect spriterect; // Rect of the player sprite, off spritesheet.xml
    float spritescale; // What scale the sprite should be rendered at
    Entity *next; // Entity is a linked list node, WSL_App contains the head
    Entity *prev; // Why is it double linked? Habit I suppose. 

    void (*update)(Entity*, WSL_App*); // Entity update function
    void (*render)(Entity*, WSL_App*); // Entity render function
    void (*deathfunc)(Entity*, WSL_App*); // Function to be called on entity destruction
};

/*****
 * Entity Creation/Destruction
 *****/
Entity* create_entity(SDL_Rect spriterect);
void destroy_entity(Entity *entity);
Entity* create_player(SDL_Rect spriterect);
Entity* create_projectile(Entity *from, SDL_Rect spriterect);
Entity* create_asteroid(void);
void spawn_asteroid(WSL_App *game);
void spawn_explosion(int x, int y, WSL_App *game);
Entity* create_particle_test(Entity *from, WSL_App *game);
void explosive_death(Entity *entity, WSL_App *game);

void update_particle(Entity *particle, WSL_App *game); //Temporarily here
void render_particle_test(Entity *particle, WSL_App *game); //Temporarily here

/*****
 * Entity utility functions
 *****/
int count_entities(Entity *entity);
SDL_Rect get_hitbox(Entity *entity);
bool entities_are_player(Entity *a, Entity *b);
bool entities_are_enemy(Entity *a, Entity *b);
bool entity_is_player(Entity *a);
bool entity_is_enemy(Entity *a);
bool check_collision_rect(SDL_Rect a, SDL_Rect b);

/*****
 * Entity drawing functions
 * (Might move to draw.h)
 *****/
void entity_render(Entity *entity, WSL_App *game);

#endif //ENTITY_H
