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
    player->speed = 8;
    player->spritescale = 0.75;
    return player;
}

Entity* create_projectile(Entity *from, SDL_Rect spriterect) {
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
    SDL_Rect spriterect;// = {651,447,43,43};
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
    asteroid->spritescale = mt_rand(75,150) / 100;
    return asteroid;
}

void entity_render(Entity *entity, WSL_App *game) {
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
