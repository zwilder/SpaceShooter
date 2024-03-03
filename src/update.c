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

void update(WSL_App *game) {
    Entity *entity = NULL, *tmp = NULL;

    // Update entities
    entity = game->entities;
    while(entity) {
        entity->update(entity,game);
        entity = entity->next;
    }

    // Cleanup list
    entity = game->entities;
    while(entity) {
        tmp = entity;
        entity = entity->next;
        if(tmp->cooldown) {
            tmp->cooldown -= 1;
        } else {
            tmp->flags &= ~EF_COOLDOWN;
        }
        if(!((tmp->flags & EF_ALIVE) == EF_ALIVE)) {
            //Entity is dead, remove it
            wsl_destroy_entity(game, tmp);
        }
    }
}
