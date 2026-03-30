#include "../game.h"
#include "menu.h"

//move to Menu
void drawMainMenu(AppState *app){
    float w, h;
    SDL_GetTextureSize(app->logoTexture, &w, &h);

    SDL_FRect dst = {(float)((WINDOW_WIDTH/2)-((w*3)/2)), 100, (w*3), (h*3)};//to make logo readable and centered on screen
    SDL_RenderTexture(app->renderer,app->logoTexture,NULL, &dst);

    loadButtons(app);//button 1
}


void loadButtons(AppState *app){
    float wB, hB;
    SDL_GetTextureSize(app->buttonOn, &wB, &hB);


    app->playbutton.w = wB;
    app->playbutton.h = hB;
    app->playbutton.x = (float)((float)((WINDOW_WIDTH/2)-((wB*3)/2.5)));
    app->playbutton.y = 500.0f;

    SDL_RenderTexture(app->renderer,app->buttonOn,NULL, &app->playbutton);
}