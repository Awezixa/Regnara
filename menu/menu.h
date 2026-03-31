#ifndef MENU_H
#define MENU_H

typedef struct AppState AppState;

void drawMainMenu(AppState *app);
void drawPauseMenu(AppState *app);
void drawOptions(AppState *app);
void drawEndScreen(AppState *app);
void playButton(AppState *app);
// void quitButton(AppState *app);
//Xavier
void drawText(AppState *app, const char *text, SDL_FRect container);

#endif 
