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

#include <timer.h>
#include <stdlib.h>
#include <stddef.h>

Timer* create_timer(int t, void (*action)(void *obj), bool repeat) {
    Timer *timer = malloc(sizeof(Timer));
    timer->initTime = t;
    timer->curTime = t;
    timer->repeat = repeat;
    timer->action = action;
    timer->next = NULL;
    timer->prev = NULL;
    return timer;
}

void destroy_timer(Timer *timer) {
    if(timer) {
        free(timer);
        timer = NULL;
    }
}

void update_timer_list(Timer *headref, void *owner) {
    /* This expects an object pointer "owner" for the timers to "act" on when
     * they reach 0 - is it weird to use void* pointers? Will this get crazy
     * unmaintainable or is it a good practice in generic programming? Unsure.*/
    if(!headref || !owner) return;
    Timer *timer = headref;
    Timer *tmp = NULL;
    while(timer) {
        timer->curTime -= 1;
        if(timer->curTime <= 0) {
            if(timer->action) {
                timer->action(owner);
            }
            if(timer->repeat) {
                timer->curTime = timer->initTime;
                timer = timer->next;
            } else {
                //destroy timer and advance to next
                tmp = timer;
                timer = timer->next;
                destroy_timer(tmp);
            }
        } else {
            // Advance
            timer = timer->next;
        }
    }
}

void add_timer_to_list(Timer **headref, Timer *timer) {
    Timer *tmp = NULL;
    if(!timer) return;
    if(!(*headref)) {
        // Timer is first (only) in list
        *headref = timer;
        return;
    }
    //Get last timer in list
    tmp = *headref;
    while(tmp->next) {
        tmp = tmp->next;
    }
    //Set the last timer's next to the new timer, and the new timer's previous
    //to the last timer.
    tmp->next = timer;
    timer->prev = tmp;
}

Timer* remove_timer_from_list(Timer **headref, Timer *timer) {
    if(!(*headref) || !timer) return NULL;
    Timer *result = *headref;
    Timer *prev = NULL;
    if(result == timer) {
        *headref = (*headref)->next;
        result->next = NULL;
        result->prev = NULL;
    } else {
        while(result != timer && result) {
            prev = result;
            result = result->next;
        }
        if(result) {
            prev->next = result->next;
            if(result->next) {
                result->next->prev = prev;
            }
            result->next = NULL;
            result->prev = NULL;
        }
    }
    return result;
}
