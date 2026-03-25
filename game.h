#ifndef GAME_H
#define GAME_H

//definitions and library includes
#define APP_NAME "Regnara"
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define TILE_SIZE 32

#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "Utils/sdl_utils.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <stdbool.h>
#include <stdlib.h>

//other file includes
#include "gameStates/gameState.h"
// We need to define this here because map.h and menu.h will use it.
// If gameState uses it, then it should be included AFTER this definition!
typedef struct AppState AppState;
#include "map/map.h"
#include "menu/menu.h"
//#include "camera/camera.h"



typedef struct InputState
{
    float mouseX;
    float mouseY;
    bool mouseLeftDown;
    bool mouseLeftPressed;
    bool mouseLeftReleased;
    bool keyDown[SDL_SCANCODE_COUNT];
    bool keyPressed[SDL_SCANCODE_COUNT];
    bool keyReleased[SDL_SCANCODE_COUNT];
} InputState;

typedef struct AppState
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;
    InputState input;
    Uint64 lastTicksMS;
    float dt;
    GameState gameState;

    //temporary texture loading
    SDL_Texture *grassTexture;
    SDL_Texture *waterTexture;
    SDL_Texture *bridgeTexture;
    SDL_Texture *logoTexture;

} AppState;


#endif
