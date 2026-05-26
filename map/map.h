#ifndef MAP_H
#define MAP_H

#include <SDL3/SDL.h>

typedef struct AppState AppState;

#define MAP_ROWS 40
#define MAP_COLS 50

#define GRASS_TILE 'G'
#define WATER_TILE 'W'
#define BRIDGE_TOP_TILE 'B'
#define BRIDGE_BOTTOM_TILE 'b'
#define TREE_TILE 'T'
#define TREE_LEFT_TILE 't'
#define TREE_RIGHT_TILE 'j'
#define UPGRADE_TILE 'U'
#define SPAWN_POINT 'S'
#define TOWN_TILE 'C'

#define SHORE_LEFT_TILE '1'
#define SHORE_RIGHT_TILE '2'
#define SHORE_TOP_TILE '3'
#define SHORE_BOTTOM_TILE '4'

#define SHORE_BOTTOM_LEFT_TILE '5'
#define SHORE_BOTTOM_RIGHT_TILE '6'
#define SHORE_TOP_LEFT_TILE '7'
#define SHORE_TOP_RIGHT_TILE '8'

#define SHORE_CORNER_TOP_LEFT_TILE '9'
#define SHORE_CORNER_TOP_RIGHT_TILE '0'
#define SHORE_CORNER_BOTTOM_LEFT_TILE '#'
#define SHORE_CORNER_BOTTOM_RIGHT_TILE '*'

extern char map_data[MAP_ROWS][MAP_COLS];
void loadMap(const char *filename);
void renderMap(SDL_Renderer *renderer, AppState *app);
int townCount(void);
void initTowns(AppState *app);
void townCaptured(AppState *app);
void drawTerritory(AppState *app);
void renderBridgeBottomOverlays(SDL_Renderer *renderer, AppState *app);
void renderTownOverlays(SDL_Renderer *renderer, AppState *app);
#endif
