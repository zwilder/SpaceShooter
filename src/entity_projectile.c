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
    proj->deathfunc = &projectile_impact_death;
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
        proj->flags |= EF_OOB;
    }

    // If the projectile hit something, kill both it and the projectile
    other = game->entities;
    while(other) {
        if((other == proj) || (entity_is_projectile(other))) {
            other = other->next;
            continue;
        }
        otherbox = get_hitbox(other);
        if(check_collision_rect(hitbox, otherbox)) {
            //CONTACT
            if((entity_is_player(proj) && entity_is_enemy (other)) ||
                   (entity_is_enemy(proj) && entity_is_player(other))) {
                // Player projectile hitting enemy, or enemy projectile hitting
                // player. Player projectiles can't hit player entities, and
                // enemy projectiles can't hit enemy entities. 
                proj->flags &= ~EF_ALIVE;
                //other->flags &= ~EF_ALIVE;
                if(other->take_damage) {
                    other->take_damage(other, game);
                } else {
                    other->flags &= ~EF_ALIVE;
                }
            } else if ((entity_is_enemy(proj) && entity_is_player(other))) {
                // Enemy projectile hitting player
                proj->flags &= ~EF_ALIVE;
                other->take_damage(other, game);
            }
        }
        other = other->next;
    }

    // Update the projectiles position
    proj->x += proj->dx * proj->speed;
    proj->y += proj->dy * proj->speed;
}

void update_projectile_flash(Entity *flash, WSL_App *game) {
    flash->frame += 1;
    flash->rgba[3] -= 25;
    if(flash->frame >= 5) {
        flash->spriterect.x = 443;
        flash->spriterect.y = 182;
        flash->spriterect.w = 48;
        flash->spriterect.h = 46;
    }
    if(flash->frame == 10) {
        flash->flags &= ~EF_ALIVE;
    }
}

void projectile_impact_death(Entity *proj, WSL_App *game) {
	//<SubTexture name="laserGreen14.png" x="193" y="240" width="48" height="46"/>
	//<SubTexture name="laserGreen15.png" x="443" y="182" width="48" height="46"/>
    if(!((proj->flags & EF_OOB) == EF_OOB)) {
        SDL_Rect spriterect = {193,240,48,46};
        Entity *flash = create_entity(spriterect);
        flash->flags = EF_ALIVE;
        flash->x = proj->x;
        flash->y = proj->y;
        flash->render = &entity_render;
        flash->update = &update_projectile_flash;
        //flash->angle = 0.15 + (0.45*genrand_real1());
        flash->angle = 0.45;
        flash->spritescale = proj->spritescale * 0.6;
        wsl_add_entity(game, flash);
        wsl_play_sound(game, mt_rand(SND_IMPACT0,SND_IMPACT4), CH_IMPACT);
    }
}
