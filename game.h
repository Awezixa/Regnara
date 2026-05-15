#ifndef GAME_H
#define GAME_H

//definitions and library includes
#define APP_NAME "Regnara"
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define TILE_SIZE 64
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
#include "pieces/piece.h"
#include "camera/camera.h"
#include "camera/transform.h"
#include "player/player.h"
#include "techTree/techTree.h"
#include "cheats/cheats.h"

typedef struct InputState
{
    //mouse
    bool mouseMiddleDown;
    bool mouseMiddlePressed;
    bool mouseMiddleReleased;
    float mouseX;
    float mouseY;
    float mouseWheelY;
    bool mouseLeftDown;
    bool mouseLeftPressed;
    bool mouseLeftReleased;
    bool mouseRightDown;
    bool mouseRightPressed;
    bool mouseRightReleased;
    //buttons
    bool keyDown[SDL_SCANCODE_COUNT];
    bool keyPressed[SDL_SCANCODE_COUNT];
    bool keyReleased[SDL_SCANCODE_COUNT];
} InputState;

typedef struct AppState
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;
    TTF_Font *fontLarge;
    InputState input;
    Uint64 lastTicksMS;
    float dt;
    GameState gameState;

    bool cheats;
    SDL_FRect cheatText;
    bool inGame;

    //other structs used/ be accessed here
    Player1 P1;
    Player2 P2;
    int currentPlayer;
    int turnCounter;
    int winner;
    int maxPlayerPieces;
    SDL_FRect p1Rect;
    SDL_FRect p2Rect;
    // Tech Tree
    SDL_FRect techText;
    TechTree techTreeP1;
    TechTree techTreeP2;

    //temporary texture loading
    //Map
    SDL_Texture *grassTexture;
    SDL_Texture *waterTexture;

    SDL_Texture *bridgeTopTexture;
    SDL_Texture *bridgeBottomTexture;
    SDL_Texture *townTexture;
    SDL_Texture *upgradePlatformTexture;

    SDL_Texture *treeTexture;
    SDL_Texture *treeLeftTexture;
    SDL_Texture *treeRightTexture;

    //shore
    SDL_Texture *shoreLeftTexture;
    SDL_Texture *shoreRightTexture;
    SDL_Texture *shoreTopTexture;
    SDL_Texture *shoreBottomTexture;
    SDL_Texture *shoreTopRightTexture;
    SDL_Texture *shoreTopLeftTexture;
    SDL_Texture *shoreBottomRightTexture;
    SDL_Texture *shoreBottomLeftTexture;
    SDL_Texture *shoreCornerTopRightTexture;
    SDL_Texture *shoreCornerTopLeftTexture;
    SDL_Texture *shoreCornerBottomRightTexture;
    SDL_Texture *shoreCornerBottomLeftTexture;

    SDL_Texture *logoTexture;
    SDL_Texture *blueGoldTexture;
    SDL_Texture *redGoldTexture;
    SDL_Texture *blueTownsTexture;
    SDL_Texture *redTownsTexture;

    Town towns[8];
    int tTowns; // total amount of towns

    //Pieces
    Piece *pieces;
    int maxPieceCapacity;
    int pieceCount;
    pieceType selectedPieceType;
    Piece *selectedPiece;
    
    //where borrow stock piece textures https://nulltale.itch.io/heroes-chess
    SDL_Texture *bluePawnTexture;
    SDL_Texture *blueRookTexture;
    SDL_Texture *blueKnightTexture;
    SDL_Texture *blueBishopTexture;
    SDL_Texture *blueKingTexture;
    SDL_Texture *blueQueenTexture;

    SDL_Texture *blueEnvoyTexture;
    SDL_Texture *blueLancerTexture;
    SDL_Texture *blueMageTexture;
    SDL_Texture *blueCatapultTexture;

    SDL_FRect pawnButton;
    SDL_FRect queenButton;
    SDL_FRect knightButton;
    SDL_FRect rookButton;
    SDL_FRect bishopButton;
    SDL_FRect envoyButton;
    SDL_FRect mageButton;
    SDL_FRect catapultButton;
    SDL_FRect lancerButton;


    //black piece textures
    SDL_Texture *redPawnTexture;
    SDL_Texture *redRookTexture;
    SDL_Texture *redKnightTexture;
    SDL_Texture *redBihsopTexture;
    SDL_Texture *redKingTexture;
    SDL_Texture *redQueenTexture;

    SDL_Texture *redEnvoyTexture;
    SDL_Texture *redLancerTexture;
    SDL_Texture *redMageTexture;
    SDL_Texture *redCatapultTexture;

    //possible move textures
    SDL_Texture *movePossible;
    SDL_Texture *moveCapture;
    SDL_Texture *moveRanged;
    SDL_Texture *moveLancer;
    
    bool piecePlaced;
    float pieceX, pieceY;

    //menu & UI textures
    SDL_Texture *buttonOn;
    SDL_Texture *buttonHovered;
    SDL_Texture *goldTexture;
    SDL_FRect playbutton;
    SDL_FRect quitbutton;
    SDL_FRect optionsbutton;
    SDL_FRect mainmenubutton;
    SDL_FRect endTurnButton;
    SDL_FRect turnCount;
    SDL_FRect pauseTxt;
    SDL_FRect techTreeButton;
    
    //camera stuff
    Camera camera;
    Vec2 worldSize;

    float camX;
    float camY;
    float pivotX;
    float pivotY;
    float errorTimer;
    SDL_Point possibleMoves[32];
    int possibleMoveCount;

    SDL_Texture *possibleMovesTexture;

    //variables for the ranged attacks
    SDL_Point possibleAttacks[64];
    int possibleAttackCount;
    
} AppState;


//other functions here
void updateGame(AppState *app); // should deal with movement spawning etc
void endTurn(AppState *app);
void winCondition(AppState *app);
void resetGame(AppState *app);
void startGame(AppState *app);
void increaseTroopLimit(AppState *app, int extraSlots);
#endif
