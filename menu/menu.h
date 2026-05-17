#ifndef MENU_H
#define MENU_H

#include "../pieces/piece.h"

typedef struct AppState AppState;

void drawOptions(AppState *app);

typedef struct
{
    pieceType baseType;
    pieceType upgradeType;

    SDL_FRect *button;

    SDL_Texture *blueAvailable;
    SDL_Texture *blueUnavailable;
    SDL_Texture *blueLocked;

    SDL_Texture *redAvailable;
    SDL_Texture *redUnavailable;
    SDL_Texture *redLocked;

} UnitUIButton;

typedef enum
{
    UI_LOCKED,
    UI_UNAVAILABLE,
    UI_AVAILABLE
} UIState;


//Xavier
void drawMainMenu(AppState *app);
void drawText(AppState *app, TTF_Font *font, const char *text, SDL_FRect container);
void endTurnButton(AppState *app);
void gameUI(AppState *app);
void quitButton(AppState *app);
void turnUI(AppState *app);
void playButton(AppState *app);
void drawPauseMenu(AppState *app);
void unitIconUI(AppState *app);
void townAmountTextUI(AppState *app, int owner, SDL_FRect container);
void drawEndScreen(AppState *app);
void mainMenuButton(AppState *app);
void optionsButton(AppState *app);
void quitPauseButton(AppState *app);
void techTreeButton(AppState *app);
void playerRectangles(AppState *app);
void drawSinglePlayerStatus(AppState *app, SDL_FRect panel, const char* title, int playerNum);

//Tiago
UIState getPieceUIState(AppState *app, pieceType type);
#endif 
