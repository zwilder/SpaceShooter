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

Entity* create_entity(SDL_Rect spriterect) {
    Entity *entity = malloc(sizeof(Entity));
    entity->spriterect = spriterect;
    entity->x = 0;
    entity->y = 0;
    entity->dx = 0;
    entity->dy = 0;
    entity->cooldown = 0;
    entity->speed = 0;
    entity->next = NULL;
    entity->prev = NULL;
    entity->flags = EF_NONE;
    return entity;
}

void destroy_entity(Entity *entity) {
    if(!entity) return;
    free(entity);
}

int count_entities(Entity *entity) {
    if(!entity) return 0;
    return (count_entities(entity->next) + 1);
}

Entity* create_player(SDL_Rect spriterect) {
    Entity *player = create_entity(spriterect);
    player->flags = EF_ALIVE | EF_PLAYER;
    player->update = &update_player;
    player->render = &entity_render;
    player->speed = 4;
    return player;
}

Entity* create_projectile(Entity *from, SDL_Rect spriterect) {
    Entity *proj = create_entity(spriterect);
    proj->flags = EF_ALIVE | EF_PROJECTILE;
    proj->render = &entity_render;
    proj->update = &update_projectile;
    proj->speed = 16;
    proj->x = from->x + (from->spriterect.w / 2);
    proj->y = from->y;
    return proj;
}

void update_player(Entity *player, WSL_App *game) {
    Entity *proj = NULL;
    SDL_Rect projrect = {845,0,13,57};

    // Update position
    if(game->up) player->y -= player->speed;
    if(game->down) player->y += player->speed;
    if(game->left) player->x -= player->speed;
    if(game->right) player->x += player->speed;

    if(player->x <= 0) player->x = 0;
    if(player->x >= (SCREEN_WIDTH - player->spriterect.w)){
        player->x = SCREEN_WIDTH - player->spriterect.w;
    }
    if(player->y <= 0) player->y = 0;
    if(player->y >= (SCREEN_HEIGHT - player->spriterect.h)) {
        player->y = SCREEN_HEIGHT - player->spriterect.h;
    }

    if(game->fire && !((player->flags & EF_COOLDOWN) == EF_COOLDOWN)) {
        proj = create_projectile(player, projrect);
        proj->flags |= EF_PLAYER;
        proj->dy = -1; // Projectile going up
        wsl_add_entity(game, proj); // Add projectile to list
        player->flags |= EF_COOLDOWN;
        player->cooldown = 50;
    }
}

void update_projectile(Entity *proj, WSL_App *game) {
    // Kill the projectile if it's out of bounds
    if((proj->x <= 0) || (proj->x >= SCREEN_WIDTH) || 
            (proj->y <= 0) || (proj->y >= SCREEN_HEIGHT)) {
        proj->flags &= ~EF_ALIVE;
    }

    // Update the projectiles position
    proj->x += proj->dx * proj->speed;
    proj->y += proj->dy * proj->speed;
}

void entity_render(Entity *entity, WSL_App *game) {
    wsl_texture_render_rect(game->spritesheet, entity->x, entity->y, 
            &entity->spriterect);
}
