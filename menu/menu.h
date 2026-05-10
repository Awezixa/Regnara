#ifndef MENU_H
#define MENU_H

typedef struct AppState AppState;

void drawOptions(AppState *app);



//Xavier
void drawMainMenu(AppState *app);
void drawText(AppState *app, const char *text, SDL_FRect container);
void endTurnButton(AppState *app);
void gameUI(AppState *app);
void quitButton(AppState *app);
void turnUI(AppState *app);
void playButton(AppState *app);
void drawPauseMenu(AppState *app);
void unitIconUI(AppState *app);
void goldUI(AppState *app);
void buildingIconUI(AppState *app);
void drawEndScreen(AppState *app);
void mainMenuButton(AppState *app);
void optionsButton(AppState *app);
void quitPauseButton(AppState *app);
void techTreeButton(AppState *app);
void playerRectangles(AppState *app);
void drawSinglePlayerStatus(AppState *app, SDL_FRect panel, SDL_Color color, const char* title, int playerNum);
#endif 
