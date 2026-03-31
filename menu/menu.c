#include "../game.h"
#include "menu.h"

//move to Menu
void drawMainMenu(AppState *app){
    float w, h;
    SDL_GetTextureSize(app->logoTexture, &w, &h);

    SDL_FRect dst = {(float)((WINDOW_WIDTH/2)-((w*3)/2)), 100, (w*3), (h*3)};//to make logo readable and centered on screen
    SDL_RenderTexture(app->renderer,app->logoTexture,NULL, &dst);

    playButton(app);//play button
    quitButton(app);
}


void playButton(AppState *app){
    float wB, hB;
    SDL_GetTextureSize(app->buttonOn, &wB, &hB);

    app->playbutton.w = wB;
    app->playbutton.h = hB;
    app->playbutton.x = (float)((float)((WINDOW_WIDTH/2)-((wB*3)/2.5)));
    app->playbutton.y = 500.0f;

    SDL_RenderTexture(app->renderer,app->buttonOn,NULL, &app->playbutton);
    drawText(app, "PLAY", app->playbutton);
}

// void quitButton(AppState *app){
//     float wB, hB;
//     SDL_GetTextureSize(app->buttonOn, &wB, &hB);

//     app->playbutton.w = wB;
//     app->playbutton.h = hB;
//     app->playbutton.x = (float)((float)((WINDOW_WIDTH/2)-((wB*3)/20)));
//     app->playbutton.y = 500.0f;

//     SDL_RenderTexture(app->renderer,app->buttonOn,NULL, &app->quitbutton);
//     drawText(app, "QUIT", app->quitbutton);
// }

//Xavier
void drawText(AppState *app, const char *text, SDL_FRect container) {
    SDL_Color black = {0, 0, 0, 255};//can change colour if needed
    
    // 1. Create a surface from the font
    SDL_Surface *surface = TTF_RenderText_Blended(app->font, text, 0, black);
    if (!surface) return;

    // 2. Create a texture from that surface
    SDL_Texture *texture = SDL_CreateTextureFromSurface(app->renderer, surface);
    SDL_DestroySurface(surface); // Clean up surface immediately
    if (!texture) return;

    // 3. Calculate centering
    float textW, textH;
    SDL_GetTextureSize(texture, &textW, &textH);
    
    SDL_FRect textRect = {
        container.x + (container.w - textW) / 2.0f,
        container.y + (container.h - textH) / 2.0f,
        textW,
        textH
    };

    // 4. Draw and then Destroy (Text textures are usually recreated or cached)
    SDL_RenderTexture(app->renderer, texture, NULL, &textRect);
    SDL_DestroyTexture(texture); 
}