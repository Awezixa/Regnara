#ifndef MENU_H
#define MENU_H

typedef struct AppState AppState;

void drawMainMenu(AppState *app);
void drawPauseMenu(AppState *app);
void drawOptions(AppState *app);
void drawEndScreen(AppState *app);
void loadButtons(AppState *app);

#endif 
