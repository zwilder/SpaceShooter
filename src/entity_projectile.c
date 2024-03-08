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

void update_projectile(Entity *proj, WSL_App *game) {
    Entity *other = NULL;
    SDL_Rect otherbox;
    SDL_Rect hitbox = get_hitbox(proj);
    // Kill the projectile if it's out of bounds
    if((proj->x <= 0) || (proj->x >= SCREEN_WIDTH) || 
            (proj->y <= 0) || (proj->y >= SCREEN_HEIGHT)) {
        proj->flags &= ~EF_ALIVE;
    }

    // If the projectile hit something, kill both it and the projectile
    other = game->entities;
    while(other) {
        if(other == proj) {
            other = other->next;
            continue;
        }
        otherbox = get_hitbox(other);
        if(check_collision_rect(hitbox, otherbox)) {
            //CONTACT
            if(entity_is_player(proj) && entity_is_enemy (other)) {
                // Player projectile hitting enemy
                proj->flags &= ~EF_ALIVE;
                //other->flags &= ~EF_ALIVE;
                if(other->take_damage) {
                    other->take_damage(other, game);
                } else {
                    other->flags &= ~EF_ALIVE;
                }
            }
        }
        other = other->next;
    }

    // Update the projectiles position
    proj->x += proj->dx * proj->speed;
    proj->y += proj->dy * proj->speed;
}
