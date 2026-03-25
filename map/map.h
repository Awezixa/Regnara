#ifndef MAP_H
#define MAP_H

#include "../game.h"


#define MAP_ROWS 25
#define MAP_COLS 20

#define GRASS_TILE 'G'
#define BRIDGE_TILE 'B'
#define WATER_TILE 'W'
#define SPAWN_POINT 'S'

extern char map_data[MAP_ROWS][MAP_COLS];
void loadMap(const char* filename);
void renderMap(SDL_Renderer *renderer, AppState *app);

#endif