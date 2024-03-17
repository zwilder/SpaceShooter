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
    entity->frame = 0;
    entity->x = 0;
    entity->y = 0;
    entity->dx = 0;
    entity->dy = 0;
    entity->rgba[0]=entity->rgba[1]=entity->rgba[2]=entity->rgba[3]=255;
    entity->angle = 0;
    entity->cooldown = 0;
    //entity->particletimer = 0;
    entity->health = 0;
    entity->speed = 0;
    entity->next = NULL;
    entity->prev = NULL;
    entity->flags = EF_NONE;
    entity->ai = NULL;
    entity->update = NULL;
    entity->render = NULL;
    entity->take_damage = NULL;
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
    if(entity->ai) {
        free(entity->ai);
        entity->ai = NULL;
    }
    free(entity);
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

bool entity_is_projectile(Entity *a) {
    /*
     * Check if Entity "a" has a projectile flag
     */
    return ((a->flags & EF_PROJECTILE) == EF_PROJECTILE);
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
     * "spritescale", and rotated based on the entities "angle". Also modulate
     * the color/alpha of the entity based on an entities rgba.
     */
    SDL_Rect renderquad;
    renderquad.x = entity->x;
    renderquad.y = entity->y;
    renderquad.w = entity->spriterect.w * entity->spritescale;
    renderquad.h = entity->spriterect.h * entity->spritescale;
    SDL_SetTextureColorMod(game->spritesheet->tex, entity->rgba[0], entity->rgba[1],
            entity->rgba[2]);
    SDL_SetTextureAlphaMod(game->spritesheet->tex, entity->rgba[3]);
    if(entity->angle) {
        SDL_RenderCopyEx(game->renderer, game->spritesheet->tex, 
                &entity->spriterect, &renderquad,
                entity->angle, NULL, SDL_FLIP_NONE);
    } else {
        SDL_RenderCopy(game->renderer, game->spritesheet->tex, 
                &entity->spriterect, &renderquad);
    }
}
