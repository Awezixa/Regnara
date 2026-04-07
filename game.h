#ifndef GAME_H
#define GAME_H

//definitions and library includes
#define APP_NAME "Regnara"
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define TILE_SIZE 40
#define WORLD_WIDTH (MAP_COLS * TILE_SIZE)
#define WORLD_HEIGHT (MAP_ROWS * TILE_SIZE)


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
typedef struct AppState AppState;// added this too all files that use appstate
#include "map/map.h"
#include "menu/menu.h"
#include "Pieces/piece.h"
#include "camera/camera.h"
#include "camera/transform.h"
#include "player/player.h"

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

    //other structs used/ be accessed here
    Player1 P1;
    Player2 P2;
    int turnCounter;

    Piece pieces[MAX_PIECES];
    int pieceCount;
    

    //temporary texture loading
    SDL_Texture *grassTexture;
    SDL_Texture *waterTexture;
    SDL_Texture *bridgeTexture;
    SDL_Texture *logoTexture;
    //where borrow stock piece textures https://nulltale.itch.io/heroes-chess
    SDL_Texture *WpawnTexture;
    SDL_Texture *WrookTexture;
    SDL_Texture *WknightTexture;
    SDL_Texture *WbishopTexture;
    SDL_Texture *WkingTexture;
    SDL_Texture *WqueenTexture;
    //black piece textures
    SDL_Texture *BpawnTexture;
    SDL_Texture *BrookTexture;
    SDL_Texture *BknightTexture;
    SDL_Texture *BbishopTexture;
    SDL_Texture *BkingTexture;
    SDL_Texture *BqueenTexture;
    bool piecePlaced;
    float pieceX, pieceY;

    //menu textures
    SDL_Texture *buttonOn;
    SDL_Texture *buttonHovered;
    SDL_FRect playbutton;
    SDL_FRect quitbutton;
    SDL_FRect endTurnButton;
    SDL_FRect turnCount;
    SDL_FRect pauseTxt;
    
    //camera stuff
    Camera camera;
    Vec2 worldSize;
    
} AppState;


//other functions here
void UpdateGame(AppState *app); // should deal with movement spawning etc


#endif
