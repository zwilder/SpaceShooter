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
    particle->frame = mt_rand(0,5); // Each particle lives for a diffent time
    particle->update = &update_particle; // Update function
    particle->render = &entity_render; // Basic entity render
    particle->deathfunc = &particle_death; // Gravity decay
    wsl_add_entity(game, particle); // Add particle to game list
}

void spawn_explosion(int x, int y, WSL_App *game) {
    int num_particles = mt_rand(25, 50);
    int max_radius = 20;
    int min_velocity = 0;
    int max_velocity = 10;
    float spritescale = 0.25 + genrand_real1();
    int i = 0;
    //SDL_Rect spriterect = {222,84,25,24}; //star2
    //SDL_Rect spriterect = {628,681,25,24}; //star1
    SDL_Rect spriterect = {576,300,24,24}; //star3
    for(i = 0; i < num_particles; i++) {
        spritescale = 0.25 + (0.75*genrand_real1());
        spawn_explosion_particle(x, y, game, spriterect,
                spritescale, max_radius, min_velocity, max_velocity,
                mt_rand(200,255),mt_rand(25,125),mt_rand(0,10),mt_rand(25,200));
    }
}

void spawn_random_color_explosion(int x, int y, WSL_App *game) {
    int num_particles = 25;
    int max_radius = 20;
    int min_velocity = 0;
    int max_velocity = 10;
    int i = 0, r = 0, g = 0, b = 0;
    int random_color = mt_rand(1,6);
    //SDL_Rect spriterect = {222,84,25,24}; //star2
    //SDL_Rect spriterect = {628,681,25,24}; //star1
    SDL_Rect spriterect = {576,300,24,24}; //star3
    for(i = 0; i < num_particles; i++) {
        switch(random_color) {
            case 1:
                //red
                r = mt_rand(200,255);
                g = mt_rand(0,25);
                b = mt_rand(0,50);
                break;
            case 2:
                //green
                r = mt_rand(0,25);
                g = mt_rand(200,255);
                b = mt_rand(0,50);
                break;
            case 3:
                //blue
                r = mt_rand(0,50);
                g = mt_rand(0,25);
                b = mt_rand(200,255);
                break;
            case 4:
                //yellow
                r = mt_rand(200,255);
                g = mt_rand(200,255);
                b = mt_rand(0,25);
                break;
            case 5:
                //orange
                r = mt_rand(200,255);
                g = mt_rand(100,150);
                b = mt_rand(0,25);
                break;
            default:
                //purple
                r = mt_rand(200,255);
                g = mt_rand(0,25);
                b = mt_rand(200,255);
                break;
        }
        spawn_explosion_particle(x, y, game, spriterect,
                0.75, max_radius, min_velocity, max_velocity,
                r,g,b,mt_rand(25,200));
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
                mt_rand(225,265),mt_rand(26,106),mt_rand(200,255),mt_rand(25,200));
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
    int num_particles = mt_rand(15,75);
    int max_radius = 5;
    int min_velocity = 0;
    int max_velocity = 5;
    int i = 0;
    float spritescale = 1.0;
    SDL_Rect spriterect;
    SDL_Rect tiny1 = {346,814,18,18}; //meteorBrown_tiny1
    SDL_Rect tiny2 = {399,814,16,15}; //meteorBrown_tiny2
    SDL_Rect small1 = {406,234,28,28}; //meteorBrown_small1
    SDL_Rect small2 = {778,587,29,26}; //meteorBrown_small2
    for(i = 0; i < num_particles; i++) {
        switch(mt_rand(1,4)) {
            case 1: spriterect = tiny1; break;
            case 2: spriterect = tiny2; break;
            case 3: spriterect = small1; break;
            default: spriterect = small2; break;
        }
        spritescale = 0.75 * genrand_real1();
        spawn_explosion_particle(x, y, game, spriterect,
                spritescale, max_radius, min_velocity, max_velocity,
                255,255,255,mt_rand(25,200));
    }
}

void firework_death(Entity *entity, WSL_App *game) {
    if(!((entity->flags & EF_OOB) == EF_OOB)) {
        spawn_random_color_explosion(entity->x, entity->y, game);
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
    particle->frame += 1; //Update frame
    particle->x += particle->dx * particle->speed; //Update x position
    particle->y += particle->dy * particle->speed; //Update y position
    
    if(particle->angle) {
        //If the particle starts off angled, spin it
        particle->angle += 45;
    }
    if((particle->rgba[3] > 25) && (particle->frame % 2 == 0)) {
        particle->rgba[3] -= 5;
    }

    if(particle->frame > 25) {
        // Kill the particle after 25 frames. The 25 should be another variable,
        // possibly set the particles->health to 25 and then compare it that
        // way?
        particle->flags &= ~EF_ALIVE;
    }
}

void particle_death(Entity *particle, WSL_App *game) {
    // Spawn a fancy NEW particle to replace this particle that "falls" down the
    // screen (with "gravity")
    // TODO FINISH this function tlater -- tinker around with dx/dy/speed
    Entity *decay = create_entity(particle->spriterect);
    decay->x = particle->x;
    decay->y = particle->y;
    
    decay->dy = particle->dy / (mt_rand(2,8)); // The closer this is to 1 the more vertical it goes
    //if(decay->dy <= 0) decay->dy *= -1; // Make sure the "dead" particles "fall"
    decay->dx = particle->dx / (mt_rand(4,8)); // The closer this is to 1 the more horizontal it goes
    
    decay->speed = mt_rand(1,3); // The farther apart these numbers are the weirder it looks
    decay->angle = 45;
    decay->spritescale = particle->spritescale;
    decay->rgba[0] = particle->rgba[0];
    decay->rgba[1] = particle->rgba[1];
    decay->rgba[2] = particle->rgba[2];
    decay->rgba[3] = particle->rgba[3];// -10;
    decay->frame = 5; // Higher this is, the shorter lived the "dead" particle (particles "die" after 25 frames)
    decay->flags = EF_ALIVE;
    decay->update = &update_particle;
    decay->render = &entity_render;
    wsl_add_entity(game, decay);
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
