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
    SDL_Texture *blueInfoBox;
    SDL_Texture *redInfoBox;

    Town towns[24];
    int tTowns; // total amount of towns

    //Pieces
    Piece *pieces;
    int maxPieceCapacity;
    int pieceCount;
    pieceType selectedPieceType;
    Piece *selectedPiece;
    
    //blue piece textures
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

    SDL_Texture *bluePawnUsedTexture;
    SDL_Texture *blueKnightUsedTexture;
    SDL_Texture *blueBishopUsedTexture;
    SDL_Texture *blueRookUsedTexture;
    SDL_Texture *blueQueenUsedTexture;
    SDL_Texture *blueKingUsedTexture;

    SDL_Texture *blueEnvoyUsedTexture;
    SDL_Texture *blueLancerUsedTexture;
    SDL_Texture *blueMageUsedTexture;
    SDL_Texture *blueCatapultUsedTexture;

    SDL_FRect pawnButton;
    SDL_FRect queenButton;
    SDL_FRect knightButton;
    SDL_FRect rookButton;
    SDL_FRect bishopButton;
    SDL_FRect envoyButton;
    SDL_FRect mageButton;
    SDL_FRect catapultButton;
    SDL_FRect lancerButton;


    //red piece textures
    SDL_Texture *redPawnTexture;
    SDL_Texture *redRookTexture;
    SDL_Texture *redKnightTexture;
    SDL_Texture *redBishopTexture;
    SDL_Texture *redKingTexture;
    SDL_Texture *redQueenTexture;

    SDL_Texture *redEnvoyTexture;
    SDL_Texture *redLancerTexture;
    SDL_Texture *redMageTexture;
    SDL_Texture *redCatapultTexture;

    SDL_Texture *redPawnUsedTexture;
    SDL_Texture *redKnightUsedTexture;
    SDL_Texture *redBishopUsedTexture;
    SDL_Texture *redRookUsedTexture;
    SDL_Texture *redQueenUsedTexture;
    SDL_Texture *redKingUsedTexture;

    SDL_Texture *redEnvoyUsedTexture;
    SDL_Texture *redLancerUsedTexture;
    SDL_Texture *redMageUsedTexture;
    SDL_Texture *redCatapultUsedTexture;

    //UILocked

    SDL_Texture *UIBluePawnLocked;
    SDL_Texture *UIBlueKnightLocked;
    SDL_Texture *UIBlueBishopLocked;
    SDL_Texture *UIBlueRookLocked;
    SDL_Texture *UIBlueQueenLocked;
    SDL_Texture *UIBlueKingLocked;

    SDL_Texture *UIBlueEnvoyLocked;
    SDL_Texture *UIBlueLancerLocked;
    SDL_Texture *UIBlueMageLocked;
    SDL_Texture *UIBlueCatapultLocked;

    SDL_Texture *UIRedPawnLocked;
    SDL_Texture *UIRedKnightLocked;
    SDL_Texture *UIRedBishopLocked;
    SDL_Texture *UIRedRookLocked;
    SDL_Texture *UIRedQueenLocked;
    SDL_Texture *UIRedKingLocked;

    SDL_Texture *UIRedEnvoyLocked;
    SDL_Texture *UIRedLancerLocked;
    SDL_Texture *UIRedMageLocked;
    SDL_Texture *UIRedCatapultLocked;

    //UIAvailable

    SDL_Texture *UIBluePawnAvailable;
    SDL_Texture *UIBlueKnightAvailable;
    SDL_Texture *UIBlueBishopAvailable;
    SDL_Texture *UIBlueRookAvailable;
    SDL_Texture *UIBlueQueenAvailable;
    SDL_Texture *UIBlueKingAvailable;

    SDL_Texture *UIBlueEnvoyAvailable;
    SDL_Texture *UIBlueLancerAvailable;
    SDL_Texture *UIBlueMageAvailable;
    SDL_Texture *UIBlueCatapultAvailable;

    SDL_Texture *UIRedPawnAvailable;
    SDL_Texture *UIRedKnightAvailable;
    SDL_Texture *UIRedBishopAvailable;
    SDL_Texture *UIRedRookAvailable;
    SDL_Texture *UIRedQueenAvailable;
    SDL_Texture *UIRedKingAvailable;

    SDL_Texture *UIRedEnvoyAvailable;
    SDL_Texture *UIRedLancerAvailable;
    SDL_Texture *UIRedMageAvailable;
    SDL_Texture *UIRedCatapultAvailable;

    //UIUnavailable

    SDL_Texture *UIBluePawnUnavailable;
    SDL_Texture *UIBlueKnightUnavailable;
    SDL_Texture *UIBlueBishopUnavailable;
    SDL_Texture *UIBlueRookUnavailable;
    SDL_Texture *UIBlueQueenUnavailable;
    SDL_Texture *UIBlueKingUnavailable;

    SDL_Texture *UIBlueEnvoyUnavailable;
    SDL_Texture *UIBlueLancerUnavailable;
    SDL_Texture *UIBlueMageUnavailable;
    SDL_Texture *UIBlueCatapultUnavailable;

    SDL_Texture *UIRedPawnUnavailable;
    SDL_Texture *UIRedKnightUnavailable;
    SDL_Texture *UIRedBishopUnavailable;
    SDL_Texture *UIRedRookUnavailable;
    SDL_Texture *UIRedQueenUnavailable;
    SDL_Texture *UIRedKingUnavailable;

    SDL_Texture *UIRedEnvoyUnavailable;
    SDL_Texture *UIRedLancerUnavailable;
    SDL_Texture *UIRedMageUnavailable;
    SDL_Texture *UIRedCatapultUnavailable;

    //techTreeLocked

    SDL_Texture *techTreeBluePawnLocked;
    SDL_Texture *techTreeBlueKnightLocked;
    SDL_Texture *techTreeBlueBishopLocked;
    SDL_Texture *techTreeBlueRookLocked;
    SDL_Texture *techTreeBlueQueenLocked;
    SDL_Texture *techTreeBlueEnvoyLocked;
    SDL_Texture *techTreeBlueLancerLocked;
    SDL_Texture *techTreeBlueMageLocked;
    SDL_Texture *techTreeBlueCatapultLocked;
    SDL_Texture *techTreeBlueGoldLocked;
    SDL_Texture *techTreeBluePiecesLocked;
    SDL_Texture *techTreeBluePlatformLocked;

    SDL_Texture *techTreeRedPawnLocked;
    SDL_Texture *techTreeRedKnightLocked;
    SDL_Texture *techTreeRedBishopLocked;
    SDL_Texture *techTreeRedRookLocked;
    SDL_Texture *techTreeRedQueenLocked;
    SDL_Texture *techTreeRedEnvoyLocked;
    SDL_Texture *techTreeRedLancerLocked;
    SDL_Texture *techTreeRedMageLocked;
    SDL_Texture *techTreeRedCatapultLocked;
    SDL_Texture *techTreeRedGoldLocked;
    SDL_Texture *techTreeRedPiecesLocked;
    SDL_Texture *techTreeRedPlatformLocked;

    //techTreeBuyable

    SDL_Texture *techTreeBluePawnBuyable;
    SDL_Texture *techTreeBlueKnightBuyable;
    SDL_Texture *techTreeBlueBishopBuyable;
    SDL_Texture *techTreeBlueRookBuyable;
    SDL_Texture *techTreeBlueQueenBuyable;
    SDL_Texture *techTreeBlueEnvoyBuyable;
    SDL_Texture *techTreeBlueLancerBuyable;
    SDL_Texture *techTreeBlueMageBuyable;
    SDL_Texture *techTreeBlueCatapultBuyable;
    SDL_Texture *techTreeBlueGoldBuyable;
    SDL_Texture *techTreeBluePiecesBuyable;
    SDL_Texture *techTreeBluePlatformBuyable;

    SDL_Texture *techTreeRedPawnBuyable;
    SDL_Texture *techTreeRedKnightBuyable;
    SDL_Texture *techTreeRedBishopBuyable;
    SDL_Texture *techTreeRedRookBuyable;
    SDL_Texture *techTreeRedQueenBuyable;
    SDL_Texture *techTreeRedEnvoyBuyable;
    SDL_Texture *techTreeRedLancerBuyable;
    SDL_Texture *techTreeRedMageBuyable;
    SDL_Texture *techTreeRedCatapultBuyable;
    SDL_Texture *techTreeRedGoldBuyable;
    SDL_Texture *techTreeRedPiecesBuyable;
    SDL_Texture *techTreeRedPlatformBuyable;

    //techTreeBought

    SDL_Texture *techTreeBluePawnBought;
    SDL_Texture *techTreeBlueKnightBought;
    SDL_Texture *techTreeBlueBishopBought;
    SDL_Texture *techTreeBlueRookBought;
    SDL_Texture *techTreeBlueQueenBought;
    SDL_Texture *techTreeBlueEnvoyBought;
    SDL_Texture *techTreeBlueLancerBought;
    SDL_Texture *techTreeBlueMageBought;
    SDL_Texture *techTreeBlueCatapultBought;
    SDL_Texture *techTreeBlueGoldBought;
    SDL_Texture *techTreeBluePiecesBought;
    SDL_Texture *techTreeBluePlatformBought;

    SDL_Texture *techTreeRedPawnBought;
    SDL_Texture *techTreeRedKnightBought;
    SDL_Texture *techTreeRedBishopBought;
    SDL_Texture *techTreeRedRookBought;
    SDL_Texture *techTreeRedQueenBought;
    SDL_Texture *techTreeRedEnvoyBought;
    SDL_Texture *techTreeRedLancerBought;
    SDL_Texture *techTreeRedMageBought;
    SDL_Texture *techTreeRedCatapultBought;
    SDL_Texture *techTreeRedGoldBought;
    SDL_Texture *techTreeRedPiecesBought;
    SDL_Texture *techTreeRedPlatformBought;

    //UIOverlays

    SDL_Texture *UIRedOverlayAvailable;
    SDL_Texture *UIBlueOverlayAvailable;
    SDL_Texture *UIRedOverlayUnavailable;
    SDL_Texture *UIBlueOverlayUnavailable;

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
    SDL_Texture *techTreeBackground;
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
