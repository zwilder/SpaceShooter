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
    EF_OOB          = 1 << 6,
    EF_INV          = 1 << 7,
    EF_BLIP         = 1 << 8,
    EF_PICKUP       = 1 << 9
} EntityFlags;

typedef struct Entity Entity;
typedef struct EntityAI EntityAI;

struct Entity {
    float x; // Will probably replace with a float, or Vec2f even 
    float y; // Entity's x,y coordinates on the screen
    float dx; // The change (delta) in the x,y coordinates
    float dy;
    double angle; // Angle the sprite is rendered at
    int speed; // How fast the entity is
    int cooldown; // Action cooldown timer 
    int frame; // Animation frame timer
    int flags; // EntityFlags
    int health; // How much health the entity has
    uint8_t rgba[4]; // Red, green, blue, alpha 
    SDL_Rect spriterect; // Rect of the player sprite, off spritesheet.xml
    float spritescale; // What scale the sprite should be rendered at
    char *txt; // Used by bliptxt, could get the $USER for the high scores?

    Entity *next; // Entity is a linked list node, WSL_App contains the head
    Entity *prev; // Why is it double linked? Habit I suppose. 

    EntityAI *ai; // AI Component

    void (*update)(Entity*, WSL_App*); // Entity update function
    void (*render)(Entity*, WSL_App*); // Entity render function
    void (*take_damage)(Entity*, WSL_App*); // What do when taking damage
    void (*deathfunc)(Entity*, WSL_App*); // Function to be called on entity destruction
};

struct EntityAI {
    Vec2f bzst; // Bezier curve points, start/mid/end
    Vec2f bzmid;// For movement, enemies move on a curve
    Vec2f bzend;
    float bzt; // Interpolation point on curve, value 0-1
    bool mvleft;
};
/*****
 * Entity Creation/Destruction - entity.c
 *****/
Entity* create_entity(SDL_Rect spriterect);
void destroy_entity(Entity *entity);

/*****
 * Entity utility functions - entity.c
 *****/
int count_entities(Entity *entity);
SDL_Rect get_hitbox(Entity *entity);
bool entities_are_player(Entity *a, Entity *b);
bool entities_are_enemy(Entity *a, Entity *b);
bool entity_is_player(Entity *a);
bool entity_is_enemy(Entity *a);
bool entity_is_projectile(Entity *a);
bool entity_is_blip(Entity *a);
bool entity_is_oob(Entity *a);
bool check_collision_rect(SDL_Rect a, SDL_Rect b);

/*****
 * Entity drawing functions - entity.c
 *****/
void entity_render(Entity *entity, WSL_App *game);

/*****
 * Player entity functions - entity_player.c
 *****/
Entity* create_player(SDL_Rect spriterect);
void update_player(Entity *player, WSL_App *game);
void player_damage(Entity *player, WSL_App *game);
void player_render(Entity *player, WSL_App *game);
Entity* find_player(WSL_App *game);

/*****
 * Projectile entity functions - entity_projectile.c
 *****/
Entity* create_projectile(Entity *from, SDL_Rect spriterect);
void update_projectile(Entity *proj, WSL_App *game);
void projectile_impact_death(Entity *proj, WSL_App *game);

/*****
 * UFO entity functions - entity_ufo.c
 *****/
Entity* create_ufo(SDL_Rect spriterect);
void spawn_ufo(WSL_App *game, Entity *from);
void ufo_update(Entity *ufo, WSL_App *game);
void ufo_damage(Entity *ufo, WSL_App *game);
void ufo_death(Entity *ufo, WSL_App *game);

/*****
 * Asteroid entity funcitons - entity_asteroid.c
 *****/
Entity* create_asteroid(void);
void spawn_asteroid(WSL_App *game);
void spawn_small_asteroid(Entity *entity, WSL_App *game);
void asteroid_death(Entity *entity, WSL_App *game);
void asteroid_sm_death(Entity *entity, WSL_App *game);
void update_asteroid(Entity *asteroid, WSL_App *game);
void asteroid_damage(Entity *asteroid, WSL_App *game);

/*****
 * Particles - entity_particles.c
 *****/
void update_particle(Entity *particle, WSL_App *game);
void particle_death(Entity *particle, WSL_App *game);
void spawn_thruster_particles(Entity *from, WSL_App *game, int qty);
void spawn_thruster_particle(Entity *from, WSL_App *game);
void render_particle_test(Entity *particle, WSL_App *game);
Entity* create_particle_test(Entity *from, WSL_App *game);

/*****
 * Explosion particles - entity_explosions.c
 *****/
void spawn_explosion_particle(int x, int y, WSL_App *game, SDL_Rect spriterect,
        float spritescale, int max_radius, int min_velocity, int max_velocity,
        uint8_t r, uint8_t g, uint8_t b, uint8_t a);
void spawn_explosion(int x, int y, WSL_App *game);
void spawn_purple_explosion(int x, int y, WSL_App *game);
void spawn_green_explosion(int x, int y, WSL_App *game);
void spawn_asteroid_explosion(int x, int y, WSL_App *game);
void spawn_random_color_explosion(int x, int y, WSL_App *game);
void firework_death(Entity *entity, WSL_App *game);

/*****
 * Blip text - entity_bliptxt.c
 *****/
void spawn_bliptxt(int x, int y, WSL_App *game, char *txt, int life, int speed,
        uint8_t r, uint8_t g, uint8_t b, uint8_t a);
void update_bliptxt(Entity *blip, WSL_App *game);
void bliptxt_render(Entity *blip, WSL_App *game);

/*****
 * Pickups
 *****/
Entity* create_pickup(SDL_Rect spriterect);
void spawn_shield_pickup(int x, int y, WSL_App *game);
void spawn_points_pickup(int x, int y, WSL_App *game);
void spawn_random_pickup(Entity *from, WSL_App *game, int chance);
void update_pickup(Entity *pickup, WSL_App *game);
void shield_pickup_death(Entity *pickup, WSL_App *game);
void points_pickup_death(Entity *pickup, WSL_App *game, int points);
void small_points_pickup_death(Entity *pickup, WSL_App *game);
void med_points_pickup_death(Entity *pickup, WSL_App *game);
void lg_points_pickup_death(Entity *pickup, WSL_App *game);

#endif //ENTITY_H
