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

#ifndef TIMER_H
#define TIMER_H

#include <stdbool.h>

typedef struct Timer Timer;

struct Timer {
    Timer *next;
    Timer *prev;

    int initTime;
    int curTime;
    bool repeat;

    void (*action)(void *obj);
};

Timer* create_timer(int t, void (*action)(void *obj), bool repeat);
void destroy_timer(Timer *timer);
void update_timer_list(Timer *headref, void *owner);
void add_timer_to_list(Timer **headref, Timer *timer);
Timer* remove_timer_from_list(Timer **headref, Timer *timer);
#endif //TIMER_H