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

/*****
 * Entity Creation/Destruction
 *****/
Entity* create_entity(SDL_Rect spriterect) {
    /*
     * Generic entity creation. Allocates memory for an entity, sets the default
     * values for all the fields, and returns the created entity. Mostly used by
     * the other entity creation functions.
     */
    Entity *entity = malloc(sizeof(Entity));
    entity->spriterect = spriterect;
    entity->spritescale = 1.0;
    entity->x = 0;
    entity->y = 0;
    entity->dx = 0;
    entity->dy = 0;
    entity->angle = 0;
    entity->cooldown = 0;
    entity->speed = 0;
    entity->next = NULL;
    entity->prev = NULL;
    entity->flags = EF_NONE;
    entity->update = NULL;
    entity->render = NULL;
    entity->deathfunc = NULL;
    return entity;
}

void destroy_entity(Entity *entity) {
    /*
     * Deallocates the memory held by the entity. Silly function for now, but
     * the entities might eventually have char* names malloc'd, or some other
     * fun things in the structure.
     */
    if(!entity) return;
    free(entity);
}

Entity* create_player(SDL_Rect spriterect) {
    /*
     * Player entity creation function. Sets the player update function, generic
     * entity rendering function, and (eventually) will set the player death
     * function. The SDL_Rect is passed in with the ambition of someday having a
     * "choose your ship:" screen or some other player creation screen.
     */
    Entity *player = create_entity(spriterect);
    player->flags = EF_ALIVE | EF_PLAYER;
    player->update = &update_player;
    player->render = &entity_render;
    player->speed = 8;
    player->spritescale = 0.75;
    return player;
}

Entity* create_projectile(Entity *from, SDL_Rect spriterect) {
    /*
     * Creates a generic projectile, spawning from the center of the "from"
     * Entity, and sets the projectile's update and render functions.
     */
    Entity *proj = create_entity(spriterect);
    proj->flags = EF_ALIVE | EF_PROJECTILE;
    proj->render = &entity_render;
    proj->update = &update_projectile;
    proj->speed = 16;
    proj->x = from->x + ((from->spriterect.w*from->spritescale) / 2);
    proj->y = from->y;
    return proj;
}

Entity* create_asteroid(void) {
    /*
     * Creates a beautiful space potato asteroid, randomly selected from 4
     * different sprites, sets a random scale and speed, and sets the
     * update/render/death functions. 
     */ 
    SDL_Rect spriterect;// = {651,447,43,43};
    float spritescale = 1.0;
    switch(mt_rand(1,4)) {
        case 1:
            spriterect.x = 224;
            spriterect.y = 664;
            spriterect.w = 101;
            spriterect.h = 84;
            break;
        case 2:
            spriterect.x = 0;
            spriterect.y = 520;
            spriterect.w = 120;
            spriterect.h =98;
            break;
        case 3:
            spriterect.x = 518;
            spriterect.y = 810;
            spriterect.w = 89;
            spriterect.h = 82;
            break;
        case 4:
            spriterect.x = 327;
            spriterect.y = 452;
            spriterect.w = 98;
            spriterect.h = 96;
            break;
        default: break;
    }
    Entity *asteroid = create_entity(spriterect);
    asteroid->flags = EF_ALIVE | EF_ENEMY;
    asteroid->render = &entity_render;
    asteroid->update = &update_asteroid;
    asteroid->speed = mt_rand(4,8);

    switch(mt_rand(1,4)) {
        case 1: spritescale = 0.75; break;
        case 2: spritescale = 1.0; break;
        case 3: spritescale = 1.25; break;
        case 4: spritescale = 1.5; break;
        default: break;
    }
    asteroid->spritescale = spritescale;
    return asteroid;
}

void spawn_asteroid(WSL_App *game) {
    /*
     * This function is called when an asteroid actually needs to be spawned in
     * the game, spawning it at the top of the screen and setting it in motion
     * towards the bottom of the screen. It then resets the timer to spawn a new
     * asteroid.
     */
    Entity *ast = create_asteroid();
    int x = mt_rand(ast->spriterect.w, SCREEN_WIDTH - ast->spriterect.w);
    //int x = SCREEN_WIDTH / 2;
    ast->x = x;
    ast->y = 0;
    ast->dy = 1;
    wsl_add_entity(game, ast); // Add asteroid to game list
    game->asteroidspawn = mt_rand(15,55); // Set timer to spawn a new asteroid
}

/*****
 * Entity utility functions
 *****/

int count_entities(Entity *entity) {
    /* 
     * Basic recursive function to count the entities in the linked list
     */
    if(!entity) return 0;
    return (count_entities(entity->next) + 1);
}

SDL_Rect get_hitbox(Entity *entity) {
    /*
     * A rectangle around the sprite, which forms the "hitbox" for collision
     * detection. It's not pixel perfect but "good enough".
     */
    SDL_Rect result = entity->spriterect;
    result.x = entity->x;
    result.y = entity->y;
    result.w *= entity->spritescale;
    result.h *= entity->spritescale;
    return result;
}

bool entities_are_player(Entity *a, Entity *b) {
    /*
     * Check if entity "a" and Entity "b" have player flags
     */
    return entity_is_player(a) && entity_is_player(b);
}

bool entities_are_enemy(Entity *a, Entity *b) {
    /*
     * Check if entity "a" and Entity "b" have enemy flags
     */
    return entity_is_enemy(a) && entity_is_enemy(b);
}

bool entity_is_player(Entity *a) {
    /*
     * Check if Entity "a" has a player flag
     */
    return (((a->flags & EF_PLAYER) == EF_PLAYER));
}

bool entity_is_enemy(Entity *a) {
    /*
     * Check if Entity "a" has an enemy flag
     */
    return (((a->flags & EF_ENEMY) == EF_ENEMY));
}

bool check_collision_rect(SDL_Rect a, SDL_Rect b) {
    /* 
     * From rect.h/c in the toolbox, check if rectangle "a" intersects rectangle
     * "b".
     */
    int ax1,ay1,ax2,ay2;
    int bx1,by1,bx2,by2;
    bool result = false;

    ax1 = a.x;
    ay1 = a.y;
    ax2 = ax1 + a.w;
    ay2 = ay1 + a.h;
    bx1 = b.x;
    by1 = b.y;
    bx2 = bx1 + b.w;
    by2 = by1 + b.h;
    
    if((ax1 <= bx2) && (ax2 >= bx1) && (ay1 <= by2) && (ay2 >= by1)) {
        result = true;
    }

    return result; 
}

/*****
 * Entity drawing functions
 * (Might move to draw.h)
 *****/
void entity_render(Entity *entity, WSL_App *game) {
    /*
     * Render an entity, with the sprite scaled based on the entity's
     * "spritescale", and rotated based on the entities "angle". May also add
     * bits to modulate the color of the entity based on an entities rgba.
     */
    SDL_Rect renderquad;
    renderquad.x = entity->x;
    renderquad.y = entity->y;
    renderquad.w = entity->spriterect.w * entity->spritescale;
    renderquad.h = entity->spriterect.h * entity->spritescale;
    if(entity->angle) {
        SDL_RenderCopyEx(game->renderer, game->spritesheet->tex, 
                &entity->spriterect, &renderquad,
                entity->angle, NULL, SDL_FLIP_NONE);
    } else {
        SDL_RenderCopy(game->renderer, game->spritesheet->tex, 
                &entity->spriterect, &renderquad);
    }
}
