#include "../game.h"
#include "menu.h"

//move to Menu
void drawMainMenu(AppState *app){
    float w, h;
    SDL_GetTextureSize(app->logoTexture, &w, &h);

    SDL_FRect dst = {(float)((WINDOW_WIDTH/2)-((w*3)/2)), 100, (w*3), (h*3)};//to make logo readable and centered on screen
    SDL_RenderTexture(app->renderer,app->logoTexture,NULL, &dst);

    





}
