#ifndef MENU_H
#define MENU_H

typedef struct AppState AppState;
//Xavier
void drawMainMenu(AppState *app);

void drawPauseMenu(AppState *app);
void drawOptions(AppState *app);
void drawEndScreen(AppState *app);
//Xavier
void playButton(AppState *app);
//Xavier
void quitButton(AppState *app);
void P1UI(AppState *app);
void P2UI (AppState *app);
void turnUI(AppState *app);
//Xavier
void endTurnButton(AppState *app);
//Xavier
void drawText(AppState *app, const char *text, SDL_FRect container);

#endif 
