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
    entity->particletimer = 0;
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
    ast->deathfunc = &firework_death;
    wsl_add_entity(game, ast); // Add asteroid to game list
    game->asteroidspawn = mt_rand(15,55); // Set timer to spawn a new asteroid
}

void firework_death(Entity *entity, WSL_App *game) {
    if(!((entity->flags & EF_OOB) == EF_OOB)) {
        switch(mt_rand(1,3)) {
            case 1:
                spawn_green_explosion(entity->x, entity->y, game);
                break;
            case 2:
                spawn_purple_explosion(entity->x, entity->y, game);
                break;
            default:
                spawn_explosion(entity->x, entity->y, game);
                break;
        }
    }
}

Entity* create_particle_test(Entity *from, WSL_App *game) {
    /* This mess of a function is basically me playing with ideas. */
    //SDL_Rect spriterect = {628,681,25,24}; //star1
    SDL_Rect spriterect = {222,84,25,24}; //star2
    //SDL_Rect spriterect = {827,125,16,40}; //fire00
	//<SubTexture name="star1.png" x="628" y="681" width="25" height="24"/>
	//<SubTexture name="star2.png" x="222" y="84" width="25" height="24"/>
	//<SubTexture name="star3.png" x="576" y="300" width="24" height="24"/>
	//<SubTexture name="fire00.png" x="827" y="125" width="16" height="40"/>
    Entity *particle = create_entity(spriterect);
    // Randomly create a particle around the "from" entity
    particle->x = from->x;// + (mt_rand(-10,10));
    particle->y = from->y;// + (mt_rand(20,30));
    particle->frame = mt_rand(0,5); // Start the frame at a random spot 
    particle->flags = EF_ALIVE;
    particle->rgba[3] = 75; // Semi transparent
    particle->spritescale = 0.75;
    //particle->angle = 180;
    // Randomly R/G/B for generic particle - may have "special" particle
    // functions to create them in certain colors?
    switch(mt_rand(1,3)) {
        case 1:
            //Blue
            particle->rgba[0] = 0;
            particle->rgba[1] = 0;
            break;
        case 2:
            //Green
            particle->rgba[0] = 0;
            particle->rgba[2] = 0;
            break;
        default:
            //Red
            particle->rgba[1] = 0;
            particle->rgba[2] = 0;
            break;
    }
    switch(mt_rand(1,3)) {
        case 1: particle->dx = mt_rand(1,2) / -2; break; //left
        case 2: particle->dx = mt_rand(1,2) / 2; break; //right
        default: particle->dx = 0; break; //straight;
    }
    particle->dy = 1;
    particle->speed = 2;
    particle->update = &update_particle;
    particle->render = &render_particle_test;
    return particle;
}

void update_particle(Entity *particle, WSL_App *game) {
    particle->frame++;
    particle->x += particle->dx * particle->speed;
    particle->y += particle->dy * particle->speed;
    //particle->x += mt_rand(-10,10); //wiggle
    if(particle->frame > 10) {
        //particle->y += 5;
    }
    
    if(particle->angle) {
        particle->angle += 45;
    }
    if(particle->frame > 25) {
        particle->flags &= ~EF_ALIVE;
    }
}

void render_particle_test(Entity *particle, WSL_App *game) {
    uint8_t ogcolor[4] = {particle->rgba[0], particle->rgba[1],
        particle->rgba[2], particle->rgba[3]};
   
    if(particle->frame % 3 == 0) {
        //Render the particle as white every other frame for **shimmer**
        particle->rgba[0] = 255;
        particle->rgba[1] = 255;
        particle->rgba[2] = 255;
        particle->rgba[3] = 255;
        entity_render(particle, game);
        particle->rgba[0] = ogcolor[0];
        particle->rgba[1] = ogcolor[1];
        particle->rgba[2] = ogcolor[2];
        particle->rgba[3] = ogcolor[3];
    } else {
        entity_render(particle,game);
    }
}

void spawn_explosion(int x, int y, WSL_App *game) {
    int num_particles = 25;
    int max_radius = 20;
    int min_velocity = 0;
    int max_velocity = 10;
    int i = 0;
    //SDL_Rect spriterect = {222,84,25,24}; //star2
    //SDL_Rect spriterect = {628,681,25,24}; //star1
    SDL_Rect spriterect = {576,300,24,24}; //star3
    for(i = 0; i < num_particles; i++) {
        spawn_explosion_particle(x, y, game, spriterect,
                0.75, max_radius, min_velocity, max_velocity,
                mt_rand(200,255),mt_rand(0,75),mt_rand(0,25),mt_rand(25,200));
    }
}

void spawn_purple_explosion(int x, int y, WSL_App *game) {
    int num_particles = 25;
    int max_radius = 20;
    int min_velocity = 0;
    int max_velocity = 10;
    int i = 0;
    //SDL_Rect spriterect = {222,84,25,24}; //star2
    //SDL_Rect spriterect = {628,681,25,24}; //star1
    SDL_Rect spriterect = {576,300,24,24}; //star3
    for(i = 0; i < num_particles; i++) {
        spawn_explosion_particle(x, y, game, spriterect,
                0.75, max_radius, min_velocity, max_velocity,
                mt_rand(200,255),mt_rand(0,75),mt_rand(200,255),mt_rand(25,200));
    }
}

void spawn_green_explosion(int x, int y, WSL_App *game) {
    int num_particles = 25;
    int max_radius = 20;
    int min_velocity = 0;
    int max_velocity = 10;
    int i = 0;
    //SDL_Rect spriterect = {222,84,25,24}; //star2
    //SDL_Rect spriterect = {628,681,25,24}; //star1
    SDL_Rect spriterect = {576,300,24,24}; //star3
    for(i = 0; i < num_particles; i++) {
        spawn_explosion_particle(x, y, game, spriterect,
                0.75, max_radius, min_velocity, max_velocity,
                mt_rand(0,25),mt_rand(200,255),mt_rand(0,75),mt_rand(25,200));
    }
}

void spawn_asteroid_explosion(int x, int y, WSL_App *game) {
    int num_particles = 25;
    int max_radius = 20;
    int min_velocity = 1;
    int max_velocity = 10;
    int i = 0;
    SDL_Rect spriterect = {346,814,18,18}; //meteorBrown_tiny1
    for(i = 0; i < num_particles; i++) {
        spawn_explosion_particle(x, y, game, spriterect,
                1.0, max_radius, min_velocity, max_velocity,
                255,255,255,mt_rand(25,200));
    }
}

void spawn_explosion_particle(int x, int y, WSL_App *game, SDL_Rect spriterect,
        float spritescale, int max_radius, int min_velocity, int max_velocity,
        uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    /*
     * This beautiful, math filled function creates a single particle and a
     * random distance about an origin point, and gives it a random velocity and
     * a random trajectory to move. It should be called by other functions
     * looking for an explosive effect multiple times in a loop for best
     * results.
     * - x,y: origin point that particles are spawning from
     * - game: pointer to the game object, which holds the list of entities
     * - spriterect: the rectangle containg the sprite on the game's spritesheet
     * - spritescale: how big the sprite in the spriterect should be rendered
     * - max_radius: how far from the origin the particle can appear
     * - min_velocity/max_velocity: The slowest and fastest a particle can move
     *   away from the origin
     * - r/g/b/a: red, green, blue, alpha colors - [0,255]
     */
    float angle = 0.0;
    float radius = 0.0;
    Entity *particle = create_entity(spriterect); // create generic entity
    angle = 2*M_PI*(float)genrand_real1(); // angle to move about origin x,y
    radius = max_radius*(float)genrand_real1(); // random distance from origin
    particle->flags = EF_ALIVE; // Particles gotta start alive
    // Polar to cartesian coordinates
    particle->x = x + radius*(float)cos(angle); // x=r*cosA
    particle->y = y + radius*(float)sin(angle); // y=r*sinA
    //Random velocity along the x axis
    particle->dx = min_velocity + (max_velocity*genrand_real1());
    if(mt_bool()) particle->dx *= -1; // about half move left, other half right
    //Random velocity along the y axis
    particle->dy = min_velocity + (max_velocity*genrand_real1());
    if(mt_bool()) particle->dy *= -1; // about half move up, other half down
    particle->speed = 1; // Basic speed
    particle->angle = 45; // Angle here is the SPRITE angle, should be passed in?
    particle->spritescale = spritescale; // Scale of the sprite used for the particle
    particle->rgba[0] = r; // Red, green, blue, and alpha used for the particle
    particle->rgba[1] = g;
    particle->rgba[2] = b;
    particle->rgba[3] = a;
    particle->update = &update_particle; // Update function
    particle->render = &entity_render; // Basic entity render
    wsl_add_entity(game, particle); // Add particle to game list
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
