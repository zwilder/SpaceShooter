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
 * WSL_App
 *****/
WSL_App* wsl_init_sdl(void) {
    bool success = true;
    int imgflags = IMG_INIT_PNG;
    int i = 0;

    WSL_App *app = malloc(sizeof(WSL_App));

    // Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize. SDL Error: %s\n", SDL_GetError());
        success = false;
    }

    // Create the window
    if(success) {
        app->window = SDL_CreateWindow("Space Shooter!",
                SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if(!app->window) {
            printf("Unable to create window. SDL Error: %s\n",
                    SDL_GetError());
            success = false;
        }
    }

    // Create the renderer
    if(success) {
        app->renderer = SDL_CreateRenderer(app->window, -1,
                SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if(!app->renderer) {
            printf("Renderer could not be created. SDL Error %s\n",
                    SDL_GetError());
            success = false;
        }
    }

    // Initialize SDL_Image
    if(success) {
        SDL_SetRenderDrawColor(app->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        if(!(IMG_Init(imgflags) & imgflags)) {
            printf("SDL Image could not initialize. SDL image error: %s\n",
                    IMG_GetError());
            success = false;
        }
    }

    // Initialize SDL_ttf
    if(success) {
        if(TTF_Init() == -1) {
            //TTF_Init returns 0 on success, -1 on failure
            printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n",
                    TTF_GetError());
            success = false;
        }
    }

    // Get the window surface
    if(success) {
        app->screen_surface = SDL_GetWindowSurface(app->window);
    }

    if(!wsl_load_media(app)) {
        success = false;
    }

    // If everything went well, return app, otherwise cleanup and return NULL
    if(!success) {
        wsl_cleanup_sdl(app);
        app = NULL;
    } else {

        app->running = true;
        app->entities = NULL;
        app->bgoffset = 0;
        app->asteroidspawn = 50;
        
        // Set keyboard flags to false
        for(i = 0; i < MAX_KEYBOARD_KEYS; i++) {
            app->keyboard[i] = false;
        }
    }
    
    return app;
}

void wsl_cleanup_sdl(WSL_App *app) {
    Entity *entity = NULL;
    if(!app) return;

    // Cleanup SDL
    destroy_wsl_texture(app->bg);
    destroy_wsl_texture(app->spritesheet);
    destroy_wsl_texture(app->ui_spritesheet);
    SDL_DestroyRenderer(app->renderer);
    app->renderer = NULL;
    SDL_DestroyWindow(app->window);
    app->window = NULL;
    IMG_Quit();
    TTF_CloseFont(app->font);
    TTF_Quit();
    SDL_Quit();

    // Cleanup entity list
    while(app->entities) {
        entity = app->entities;
        app->entities = app->entities->next;
        destroy_entity(entity);
    }

    free(app);
}

bool wsl_load_media(WSL_App *app) {
    bool success = true;
    app->spritesheet = create_wsl_texture(app->renderer);
    if(!wsl_texture_load(app->spritesheet,"assets/spritesheet.png")) {
        printf("Unable to load assets/spritesheet.png!\n");
        success = false;
    }
    app->ui_spritesheet = create_wsl_texture(app->renderer);
    if(!wsl_texture_load(app->ui_spritesheet,"assets/uipackSpace_sheet.png")) {
        printf("Unable to load assets/uipackSpace_sheet.png!\n");
        success = false;
    }
    app->bg = create_wsl_texture(app->renderer);
    if(!wsl_texture_load(app->bg, "assets/black.png")) {
        printf("Unable to load assets/black.png!\n");
        success = false;
    }
    app->font = TTF_OpenFont("assets/kenvector_future.ttf",FONT_SIZE);
    if(!app->font) {
        printf("Unable to load assets/kenvector_future.ttf!\n");
        success = false;
    }
    app->hud_text = create_wsl_texture(app->renderer);
    return success;
}

void wsl_add_entity(WSL_App *app, Entity *entity) {
    Entity *e = NULL;
    if(!app || !entity) return;
    if(!app->entities) {
        // First entity in list!
        app->entities = entity;
        return;
    }
    // Find last entity, set it's next to entity
    e = app->entities;
    while(e->next) {
        e = e->next;
    }
    e->next = entity;
    entity->prev = e;
}

Entity* wsl_remove_entity(WSL_App *app, Entity *entity) {
    if(!app || !entity) return NULL;
    if(!app->entities) return NULL;
    Entity *e = app->entities;
    Entity *prev = NULL;
    if(e == entity) {
        app->entities = app->entities->next;
        e->next = NULL;
        e->prev = NULL;
    } else {
        while(e != entity && e) {
            prev = e;
            e = e->next;
        }
        if(e) {
            prev->next = e->next;
            if(e->next) {
                e->next->prev = prev;
            }
            e->next = NULL;
            e->prev = NULL;
        }
    }
    return e;
}

void wsl_destroy_entity(WSL_App *app, Entity *entity) {
    if(!app) {
        destroy_entity(entity);
        return;
    } 
    if(!entity) return;
    // Remove entity from game list
    wsl_remove_entity(app, entity);
    // Destroy entity
    destroy_entity(entity);
}

/*****
 * WSL_Texture
 *****/
WSL_Texture* create_wsl_texture(SDL_Renderer *renderer) {
    WSL_Texture *t = malloc(sizeof(WSL_Texture));
    t->tex = NULL;
    t->w = 0;
    t->h = 0;
    t->renderer = renderer;
    return t;
}

void destroy_wsl_texture(WSL_Texture *t) {
    if(!t) return;
    if(t->tex) {
        SDL_DestroyTexture(t->tex);
    }
    free(t);
    t = NULL;
}

bool wsl_texture_load(WSL_Texture *t, char *path) {
    SDL_Surface *loaded = NULL;
    
    if(!t) return false;
    if(t->tex) {
        SDL_DestroyTexture(t->tex);
        t->tex = NULL;
    }
    
    loaded = IMG_Load(path);
    if(!loaded) {
        printf("Unable to load image %s! SDL_image Error: %s\n",
                path, IMG_GetError());
        return false;
    }

    // 0: nearest, 1: linear, 2: best
    if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
    {
        printf("Warning: Linear texture filtering not enabled!");
    }

    SDL_SetColorKey(loaded, SDL_TRUE, SDL_MapRGB(loaded->format, 0, 0xFF, 0xFF));
    t->tex = SDL_CreateTextureFromSurface(t->renderer, loaded);
    if(!t->tex) {
        printf("Unable to create texture from %s! SDL Error: %s\n", path,
                SDL_GetError());
        return false;
    }
    t->w = loaded->w;
    t->h = loaded->h;

    SDL_FreeSurface(loaded);
    return true;
}

bool wsl_texture_load_text(WSL_App *app, WSL_Texture *t, SDL_Color color, char *fstr, ...) {
    if(!fstr) return false;
    if(!app) return false;
    va_list args;
    va_start(args, fstr);
    int i = strlen(fstr) + 1;
    SDL_Surface *text_surface = NULL;
    char *str = malloc(i * sizeof(char));
    vsnprintf(str,i,fstr,args);
    va_end(args);
    if(t->tex) {
        SDL_DestroyTexture(t->tex);
        t->tex = NULL;
    }
    text_surface = TTF_RenderText_Solid(app->font, str, color);
    if(!text_surface) {
        printf("Error rendering text: \"%s\", SDL_Error: %s\n",
                str, SDL_GetError());
    } else {
        t->tex = SDL_CreateTextureFromSurface(app->renderer, text_surface);
        if(!t->tex) {
            printf("Unable to create texture from rendered text! SDL_Error: %s\n",
                    SDL_GetError());
        } else {
            t->w = text_surface->w;
            t->h = text_surface->h;
        }
        SDL_FreeSurface(text_surface);
    }
    free(str);
    return (t->tex != NULL);
}

void wsl_texture_render(WSL_Texture *t, int x, int y) {
    SDL_Rect renderquad;
    renderquad.x = x;
    renderquad.y = y;
    renderquad.w = t->w;
    renderquad.h = t->h;
    SDL_RenderCopy(t->renderer, t->tex, NULL, &renderquad);
}

void wsl_texture_render_rect(WSL_Texture *t, int x, int y, SDL_Rect *rect) {
    SDL_Rect renderquad;
    if(!rect) {
        wsl_texture_render(t, x, y);
    } else {
        renderquad.x = x;
        renderquad.y = y;
        renderquad.w = rect->w;
        renderquad.h = rect->h;
        SDL_RenderCopy(t->renderer, t->tex, rect, &renderquad);
    }
}

void wsl_texture_render_rect_scaled(WSL_Texture *t, int x, int y, SDL_Rect *rect, float scale) {
    SDL_Rect renderquad;
    if(!rect) {
        wsl_texture_render(t, x, y);
    } else {
        renderquad.x = x;
        renderquad.y = y;
        renderquad.w = (rect->w) * scale;
        renderquad.h = (rect->h) * scale;
        SDL_RenderCopy(t->renderer, t->tex, rect, &renderquad);
    }
}
