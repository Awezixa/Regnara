#include "../game.h"
#include "menu.h"

//move to Menu
void drawMenu(AppState *app){
    SDL_FRect dst = {(float)(WINDOW_WIDTH/2), (float)(WINDOW_HEIGHT/2), (float)app->logoTexture->w, (float)app->logoTexture->h};
    SDL_RenderTexture(app->renderer,app->logoTexture,NULL, &dst);

    //SDL_DestroyTexture(app->logoTexture);
}
