/*
* Toolbox
* Copyright (C) Zach Wilder 2022-2024
* 
* This file is a part of Toolbox
*
* Toolbox is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* Toolbox is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with Toolbox.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <vec2f.h>

Vec2f make_vec2f(float x, float y) {
    Vec2f result = {x,y};
    return result;
}

Vec2f add_vec2f(Vec2f a, Vec2f b) {
    return (make_vec2f(a.x + b.x, a.y + b.y));
}

Vec2f subtract_vec2f(Vec2f a, Vec2f b) {
    return (make_vec2f(a.x - b.x, a.y - b.y));
}

bool eq_vec2f(Vec2f a, Vec2f b) {
    return((a.x == b.x) && (a.y == b.y));
}

float lerp_f(float a, float b, float t) {
    /* Floating linear interpolation point, floating lerp */
    return(a + ((b-a) * t));
}

Vec2f lerp_vec2f(Vec2f a, Vec2f b, float t) {
    return(make_vec2f(lerp_f(a.x,b.x,t),lerp_f(a.y,b.y,t)));
}

Vec2f get_vec2f_on_bezier(Vec2f start, Vec2f mid, Vec2f end, float t) {
    /* Lerp the lerps to get a lerp */
    return(lerp_vec2f(lerp_vec2f(start,mid,t),lerp_vec2f(mid,end,t),t));
}
