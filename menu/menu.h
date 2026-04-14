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
#endif 
