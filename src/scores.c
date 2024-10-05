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
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

int highscore_comparator(const void *a, const void *b);

bool save_scores(WSL_App *app) {
    bool success = true;
    int i = 0;
    struct stat st = {0};
    char *pathstr = malloc(180 * sizeof(char));
    snprintf(pathstr, 180, "%s/.zwsl", getenv("HOME"));
    if(stat(pathstr, &st) == -1) {
        mkdir(pathstr, 0700);
    }
    snprintf(pathstr, 180, "%s/.zwsl/spaceshooter.bin", getenv("HOME"));
    FILE *f = fopen(pathstr,"wb+");
    if(f) {
        for(i = 0; i < NUM_HIGHSCORES; i++) {
            // Loop through app->scores writing in order
            // recent
            //printf("Writing %d\n",i);
            fwrite(&(app->scores[i].recent),sizeof(bool),1,f);
            //printf("\tRecent: %s\n", app->scores[i].recent ? "True" : "False");
            // score
            fwrite(&(app->scores[i].score),sizeof(int),1,f);
            //printf("\tScore: %d\n", app->scores[i].score);
            // namelength
            fwrite(&(app->scores[i].namelength),sizeof(int),1,f);
            //printf("\tNamelength: %d\n", app->scores[i].namelength);
            // name    
            fwrite(app->scores[i].name,sizeof(char),app->scores[i].namelength,f);
            //printf("\tName: %s\n", app->scores[i].name);
        }
    } else {
        success = false;
    }

    fclose(f);
    free(pathstr);
    return success;
}

bool load_scores(WSL_App *app) {
    int i = 0;
    int namesize = 0;
    bool success = true;
    char *pathstr = malloc(180 * sizeof(char));
    snprintf(pathstr, 180, "%s/.zwsl/spaceshooter.bin", getenv("HOME"));
    FILE *f = fopen(pathstr,"rb+");
    int bytesread = 0;

    if(!f) {
        create_scores(app);
    } else {
        for(i = 0; i < NUM_HIGHSCORES; i++) {
            //printf("%d: Loading %d:\n",bytesread,i);
            bytesread += fread(&(app->scores[i].recent),sizeof(bool),1,f);
            //printf("%d:\tRecent: %s\n", bytesread, 
            //        app->scores[i].recent ? "true" : "false");
            bytesread += fread(&(app->scores[i].score),sizeof(int),1,f);
            //printf("%d:\tScore: %d\n", bytesread, app->scores[i].score);
            bytesread += fread(&namesize,sizeof(int),1,f);
            app->scores[i].namelength = namesize;
            //printf("%d:\tNamelength: %d\n", bytesread, app->scores[i].namelength);
            app->scores[i].name = malloc(sizeof(char) * app->scores[i].namelength);
            bytesread += fread(app->scores[i].name,sizeof(char),
                    app->scores[i].namelength,f);
            //printf("%d:\tName: %s\n", bytesread, app->scores[i].name);
        }
        fclose(f);
    }

    return success;    
}

void create_scores(WSL_App *app) {
    int i = 0;
    for(i = 0; i < NUM_HIGHSCORES; i++) {
        app->scores[i].recent = false;
        app->scores[i].score = 1500+(1500 * i);
        app->scores[i].name = malloc(30 * sizeof(char));
        snprintf(app->scores[i].name, 20, "Cadet %d", mt_rand(1000,7500));
        app->scores[i].namelength = strlen(app->scores[i].name) + 1;
    }
}

void close_scores(WSL_App *app) {
    // Free memory allocated to scores
    int i = 0;
    for(i = 0; i < NUM_HIGHSCORES; i++) {
        if(app->scores[i].name) {
            free(app->scores[i].name);
        }
    }
    if(app->scores) {
        free(app->scores);
    }
}

void add_score(WSL_App *app, int score) {
    // Eventually this should accept/call a name entry thing. Temporarily just
    // using the $USER
    char *str = malloc(180 *sizeof(char));
    Highscore newscores[NUM_HIGHSCORES + 1];
    int i;

    for(i = 0; i < NUM_HIGHSCORES; i++) {
        newscores[i] = app->scores[i];
        newscores[i].recent = false;
    }

    if(score > 15000) {
        snprintf(str,180,"Captain %s",getenv("USER"));
    } else if (score > 10000) {
        snprintf(str,180,"Lieutenant %s",getenv("USER"));
    } else if (score > 5000) {
        snprintf(str,180,"Ensign %s",getenv("USER"));
    } else {
        snprintf(str,180,"Cadet %s",getenv("USER"));
    } 

    newscores[NUM_HIGHSCORES].namelength = strlen(str) + 1;
    newscores[NUM_HIGHSCORES].name = malloc(sizeof(char) * strlen(str) + 1);
    snprintf(newscores[NUM_HIGHSCORES].name, strlen(str) + 1, "%s", str);
    newscores[NUM_HIGHSCORES].score = score;
    newscores[NUM_HIGHSCORES].recent = true;

    qsort(&newscores, NUM_HIGHSCORES + 1, sizeof(Highscore), highscore_comparator);

    for(i = 0; i < NUM_HIGHSCORES; i++) {
        app->scores[i].recent = newscores[i].recent;
        app->scores[i].score = newscores[i].score;
        app->scores[i].namelength = newscores[i].namelength;
        app->scores[i].name = newscores[i].name;
    }

    // Last score in newscores **should** be the lowest score getting bumped off
    // the table, so we want to make sure to cleanup the allocated memory for
    // that string here
    if(newscores[NUM_HIGHSCORES].name) {
        free(newscores[NUM_HIGHSCORES].name);
    }
    if(str) {
        free(str);
    }
}

int highscore_comparator(const void *a, const void *b) {
    Highscore *h1 = ((Highscore*)a);
    Highscore *h2 = ((Highscore*)b);
    return h2->score - h1->score;
}
