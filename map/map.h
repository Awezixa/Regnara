#ifndef MAP_H
#define MAP_H

#include <SDL3/SDL.h>

typedef struct AppState AppState;

#define MAP_ROWS 20
#define MAP_COLS 25

#define GRASS_TILE 'G'
#define BRIDGE_TILE 'B'
#define WATER_TILE 'W'
#define SPAWN_POINT 'S'
#define TOWN_TILE 'T'

extern char map_data[MAP_ROWS][MAP_COLS];
void loadMap(const char *filename);
void renderMap(SDL_Renderer *renderer, AppState *app);
int townCount(void);


#endif
