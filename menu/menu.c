#include "../game.h"
#include "menu.h"

//move to Menu. Xavier
void drawMainMenu(AppState *app){
    float w, h;
    SDL_GetTextureSize(app->logoTexture, &w, &h);

    SDL_FRect dst = {(float)((WINDOW_WIDTH/2)-((w*3)/2)), 100, (w*3), (h*3)};//to make logo readable and centered on screen
    SDL_RenderTexture(app->renderer,app->logoTexture,NULL, &dst);

    playButton(app);//play button
    quitButton(app);
}

//Xavier
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

//Xavier
void quitButton(AppState *app){
    float wB, hB;
    SDL_GetTextureSize(app->buttonHovered, &wB, &hB);

    app->quitbutton.w = wB;
    app->quitbutton.h = hB;
    app->quitbutton.x = (float)((float)((WINDOW_WIDTH/2)-((wB*3)/20)));
    app->quitbutton.y = 500.0f;

    SDL_RenderTexture(app->renderer,app->buttonHovered,NULL, &app->quitbutton);
    drawText(app, "QUIT", app->quitbutton);
}

//Xavier
void drawText(AppState *app, const char *text, SDL_FRect container) {
    SDL_Color black = {255, 255, 255, 255};//can change colour if needed
    
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

//Xavier
void endTurnButton(AppState *app){
    float wB, hB;
    SDL_GetTextureSize(app->buttonOn, &wB, &hB);

    float scale = 1.5f;

    app->endTurnButton.w = wB *scale;
    app->endTurnButton.h = hB* scale;
    app->endTurnButton.x = WINDOW_WIDTH - app->endTurnButton.w - 20.0f;
    app->endTurnButton.y = WINDOW_HEIGHT - app->endTurnButton.h - 20.0f;

    SDL_RenderTexture(app->renderer,app->buttonOn,NULL, &app->endTurnButton);
    drawText(app, "END TURN", app->endTurnButton);
}

void turnUI(AppState *app){
    char turnStr[100];
    snprintf(turnStr, sizeof(turnStr), "Turn: %d", app->turnCounter);

    app->turnCount.x = (float)(WINDOW_WIDTH/2);
    app->turnCount.y = 20.0f;
    app->turnCount.h = 50.0f;
    app->turnCount.w = 180.0f;

    drawText(app, turnStr, app->turnCount);
}

void drawPauseMenu(AppState *app){
    char pauseUI[32];
    snprintf(pauseUI, sizeof(pauseUI), "PAUSED");

    app->pauseTxt.x = (float)(WINDOW_WIDTH/2) - 10.0f;
    app->pauseTxt.y = 200.0f;
    app->pauseTxt.h = 50.0f;
    app->pauseTxt.w = 180.0f;

    drawText(app, pauseUI, app->pauseTxt);
}