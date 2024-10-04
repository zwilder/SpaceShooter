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

void spawn_thruster_particles(Entity *from, WSL_App *game, int qty) {
    int i;
    for(i = 0; i < qty; i++) {
        spawn_thruster_particle(from,game);
    }
}

void spawn_thruster_particle(Entity *from, WSL_App *game) {
    /*
     * Spawn a single thruster particle from the middle/bottom of an entity and
     * send it off in a short "cone" towards the bottom of the screen. Should be
     * called multiple times for maximum effect.
     */
    int max_velocity = 2; // Fiddle with this, should be a random number
    int min_velocity = 0; // Fiddle with this, should be a random number
    float x = from->x + ((from->spriterect.w * from->spritescale)/2);
    float y = from->y + (from->spriterect.h * from->spritescale);
	//<SubTexture name="star1.png" x="628" y="681" width="25" height="24"/>
	//<SubTexture name="star2.png" x="222" y="84" width="25" height="24"/>
	//<SubTexture name="star3.png" x="576" y="300" width="24" height="24"/>
    SDL_Rect spriterect = {628,681,25,24};
    Entity *particle = create_entity(spriterect); // create generic entity
    particle->x = x + mt_rand(-5,5); //Slight deviation so it's slightly different
    particle->y = y;
    //particle->spritescale = 0.083; //Tiny, about 2px EH
    //particle->spritescale = 0.5; //Huge, about 12px COOL
    particle->spritescale = 0.25; //About 6px (sweet spot!)
    particle->flags = EF_ALIVE; // Particles gotta start alive
    //Send the particles down and maybe to the left/right
    particle->dy = min_velocity + (max_velocity*genrand_real1());
    if(from->angle < 0) {
        //From is pointed left, shoot particle right
        particle->dx = mt_rand(2,24) / mt_rand(2,4);
    } else if (from->angle > 0) {
        //From is pointed right, shoot particle left
        particle->dx = mt_rand(2,24) / mt_rand(-4,-2);
    } else {
        //Shoot particle down
        particle->dx = mt_rand(-1,1) / (mt_rand(1,4)); 
    }
    particle->speed = 1; //Basic speed
    particle->angle = 45; // Angle here is the SPRITE angle, should be passed in?
    particle->rgba[0] = mt_rand(225,255); // Red, green, blue, and alpha used for the particle
    particle->rgba[1] = mt_rand(0,155); // Make it orangeish?
    particle->rgba[2] = 0;
    particle->rgba[3] = mt_rand(100,200);
    particle->frame = mt_rand(0,5); // Each particle lives for a diffent time
    particle->update = &update_particle; // Update function
    particle->render = &entity_render; // Basic entity render
    particle->deathfunc = &particle_death; // Gravity decay
    wsl_add_entity(game, particle);
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

