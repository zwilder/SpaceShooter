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
    asteroid->take_damage = &asteroid_damage;
    asteroid->deathfunc = &asteroid_death;
    asteroid->speed = mt_rand(4,8);

    switch(mt_rand(1,4)) {
        case 1: 
            spritescale = 0.75; 
            asteroid->health = 1;
            break;
        case 2:
            spritescale = 1.0; 
            asteroid->health = 1;
            break;
        case 3:
            spritescale = 1.25; 
            asteroid->health = 2;
            break;
        default:
            spritescale = 1.5;
            asteroid->health = 3;
            break;
    }
    asteroid->spritescale = spritescale;
    return asteroid;
}

void update_asteroid(Entity *asteroid, WSL_App *game) {
    // Kill the asteroid if it's out of bounds
    if((asteroid->x <= 0) || (asteroid->x >= SCREEN_WIDTH) || 
            (asteroid->y >= SCREEN_HEIGHT)) {
        asteroid->flags &= ~EF_ALIVE;
        asteroid->flags |= EF_OOB;
    }

    if(!((asteroid->flags & EF_COOLDOWN) == EF_COOLDOWN)) {
        asteroid->angle += 5;
        asteroid->cooldown = 1;
        asteroid->flags |= EF_COOLDOWN;
    }
    asteroid->x += asteroid->dx * asteroid->speed;
    asteroid->y += asteroid->dy * asteroid->speed;
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

void asteroid_damage(Entity *asteroid, WSL_App *game) {
    asteroid->health -= 1;
    asteroid->speed -= 2;
    asteroid->cooldown += 2;
    if(asteroid->health <= 0) {
        asteroid->flags &= ~EF_ALIVE;
    }
}

void asteroid_death(Entity *entity, WSL_App *game) {
    if(!((entity->flags & EF_OOB) == EF_OOB)) {
        spawn_asteroid_explosion(entity->x, entity->y, game);
        //spawn_explosion(entity->x, entity->y, game);
        //spawn_random_color_explosion(entity->x, entity->y, game);
        //spawn_asteroid_particles(entity, WSL_App *game);
        //spawn_aliens_lol
        //spawn_brown_smoke(entity, WSL_App *game);
    }
}
