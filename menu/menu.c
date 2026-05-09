#include "../game.h"
#include "menu.h"

// move to Menu. Xavier
void drawMainMenu(AppState *app)
{
    float w, h;
    SDL_GetTextureSize(app->logoTexture, &w, &h);

    SDL_FRect dst = {(float)((WINDOW_WIDTH / 2) - ((w * 3) / 2)), 100, (w * 3), (h * 3)}; // to make logo readable and centered on screen
    SDL_RenderTexture(app->renderer, app->logoTexture, NULL, &dst);

    playButton(app); // play button
    quitButton(app);
}

// Xavier
void playButton(AppState *app)
{
    float wB, hB;
    SDL_GetTextureSize(app->buttonOn, &wB, &hB);

    app->playbutton.w = wB;
    app->playbutton.h = hB;
    app->playbutton.x = (float)(WINDOW_WIDTH / 2) - app->playbutton.w - 50;
    app->playbutton.y = 500.0f;

    SDL_RenderTexture(app->renderer, app->buttonOn, NULL, &app->playbutton);
    drawText(app, "PLAY", app->playbutton);
}

// Xavier
void quitButton(AppState *app)
{
    float wB, hB;
    SDL_GetTextureSize(app->buttonHovered, &wB, &hB);

    app->quitbutton.w = wB;
    app->quitbutton.h = hB;
    app->quitbutton.x = (float)(WINDOW_WIDTH / 2);
    app->quitbutton.y = 500.0f;

    SDL_RenderTexture(app->renderer, app->buttonHovered, NULL, &app->quitbutton);
    drawText(app, "QUIT", app->quitbutton);
}

// Xavier
void drawText(AppState *app, const char *text, SDL_FRect container)
{
    SDL_Color black = {255, 255, 255, 255}; // can change colour if needed

    // 1. Create a surface from the font
    SDL_Surface *surface = TTF_RenderText_Blended(app->font, text, 0, black);
    if (!surface)
        return;

    // 2. Create a texture from that surface
    SDL_Texture *texture = SDL_CreateTextureFromSurface(app->renderer, surface);
    SDL_DestroySurface(surface); // Clean up surface immediately
    if (!texture)
        return;

    // 3. Calculate centering
    float textW, textH;
    SDL_GetTextureSize(texture, &textW, &textH);

    SDL_FRect textRect = {
        container.x + (container.w - textW) / 2.0f,
        container.y + (container.h - textH) / 2.0f,
        textW,
        textH};

    // 4. Draw and then Destroy (Text textures are usually recreated or cached)
    SDL_RenderTexture(app->renderer, texture, NULL, &textRect);
    SDL_DestroyTexture(texture);
}

// Xavier
void endTurnButton(AppState *app)
{
    float wB, hB;
    SDL_GetTextureSize(app->buttonOn, &wB, &hB);

    float scale = 1.0f;

    app->endTurnButton.w = wB * scale;
    app->endTurnButton.h = hB * scale;
    app->endTurnButton.x = WINDOW_WIDTH - app->endTurnButton.w - 20.0f;
    app->endTurnButton.y = WINDOW_HEIGHT - app->endTurnButton.h - 20.0f;

    SDL_RenderTexture(app->renderer, app->buttonOn, NULL, &app->endTurnButton);
    drawText(app, "END TURN", app->endTurnButton);
}

void turnUI(AppState *app)
{
    char turnStr[100];
    snprintf(turnStr, sizeof(turnStr), "| Player %d || Turn %d |", app->currentPlayer, app->turnCounter);

    app->turnCount.x = (float)(WINDOW_WIDTH / 2) - 50.0f;
    app->turnCount.y = 20.0f;
    app->turnCount.h = 60.0f;
    app->turnCount.w = 200.0f;

    drawText(app, turnStr, app->turnCount);
}

void drawPauseMenu(AppState *app)
{
    char pauseUI[32];
    snprintf(pauseUI, sizeof(pauseUI), "PAUSED");

    app->pauseTxt.h = 50.0f;
    app->pauseTxt.w = 180.0f;
    app->pauseTxt.x = (float)(WINDOW_WIDTH / 2) - (app->pauseTxt.w/2);
    app->pauseTxt.y = 200.0f;

    drawText(app, pauseUI, app->pauseTxt);
    // add options button
    // add resume button
    quitPauseButton(app);
    mainMenuButton(app);
    optionsButton(app);
}

void gameUI(AppState *app)
{

    SDL_FRect uiPanel = {
        0, // starts left edge of screen
        WINDOW_HEIGHT - 150.0f,
        WINDOW_WIDTH,
        150.0f};
    SDL_SetRenderDrawBlendMode(app->renderer, SDL_BLENDMODE_BLEND);

    if(app->currentPlayer == 1){
        SDL_SetRenderDrawColor(app->renderer, 0, 153, 255, 180);
    }
    else if(app->currentPlayer == 2){
        SDL_SetRenderDrawColor(app->renderer, 255, 51, 0, 180);
    }
    

    SDL_RenderFillRect(app->renderer, &uiPanel);

    // add outline to the panel
    SDL_SetRenderDrawColor(app->renderer, 200, 200, 200, 255); // Solid light gray
    SDL_RenderRect(app->renderer, &uiPanel);
    unitIconUI(app);
    buildingIconUI(app);
    techTreeButton(app);
    endTurnButton(app);
}

void unitIconUI(AppState *app)
{
    pieceType types[] = {PAWN, KNIGHT, BISHOP, ROOK, QUEEN};
    SDL_Texture* texturesP1[] = {app->WpawnTexture, app->WknightTexture, app->WbishopTexture, app->WrookTexture, app->WqueenTexture};
    SDL_Texture* texturesP2[] = {app->BpawnTexture, app->BknightTexture, app->BbishopTexture, app->BrookTexture, app->BqueenTexture};
    SDL_FRect* buttons[] = {&app->pawnButton, &app->knightButton, &app->bishopButton, &app->rookButton, &app->queenButton};
    
    float spacing = 90.0f;
    float startX = (WINDOW_WIDTH / 2.0f) - ((spacing * 5) / 2.0f);
    float size = 64.0f;
    

    for (int i = 0; i < 5; i++)
    {
        buttons[i]->x = startX + (i *spacing);
        buttons[i]->y = WINDOW_HEIGHT - 110.0f;
        buttons[i]->w = size;
        buttons[i]->h = size;

        if (app->selectedPieceType == types[i])
        {
            SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, 100);
            SDL_RenderFillRect(app->renderer, buttons[i]);
        }

        SDL_Texture* tex = (app->currentPlayer == 1) ? texturesP1[i] : texturesP2[i];
        SDL_RenderTexture(app->renderer, tex, NULL, buttons[i]);

        char cost[16];
        snprintf(cost, sizeof(cost), "%d ", pieceCost(types[i]));
        SDL_FRect costPos = {buttons[i]->x, buttons[i]->y + size, size, 20.0f};
        drawText(app, cost, costPos);
    }
}
    

void buildingIconUI(AppState * app)
    {
        char countStr[32];

        int towns = (app->currentPlayer == 1) ? app->P1.towns : app->P2.towns;

        snprintf(countStr, sizeof(countStr), " %d", towns);
        SDL_FRect countPos = {
            600.0f,                 // X
            WINDOW_HEIGHT - 125.0f, // Y (below Turn UI)
            150.0f,                 // Width
            150.0f                  // Height
        };

        SDL_FRect townIcon = {
            580.0f,
            WINDOW_HEIGHT - 80.0f,
            65.0f,
            65.0f};

        SDL_RenderTexture(app->renderer, app->townTexture, NULL, &townIcon);
        drawText(app, countStr, countPos);
    }

void goldUI(AppState * app)
{
    SDL_FRect goldPanel = {
        800.0f,                 // x position
        WINDOW_HEIGHT - 207.0f, // y position
        300.0f,                 // width
        60.0f,                  // height
    };

    // gold amount
    char goldAmmount[150];
    int gold = (app->currentPlayer == 1) ? app->P1.p1Gold : app->P2.p2Gold;

    snprintf(goldAmmount, sizeof(goldAmmount), ": %d", gold);
    SDL_FRect textPos = {
        900.0f,                 // X padding from left
        WINDOW_HEIGHT - 250.0f, // Y position inside panel
        140.0f,                 // Width
        140.0f                  // Height
    };

    SDL_FRect icon = {850.0f, WINDOW_HEIGHT - 230.0f, 80.f, 80.0f};

    // for rectangle loading
    SDL_SetRenderDrawBlendMode(app->renderer, SDL_BLENDMODE_BLEND);
    if(app->currentPlayer == 1){
        SDL_SetRenderDrawColor(app->renderer, 0, 153, 255, 180);
    }
    else if(app->currentPlayer == 2){
        SDL_SetRenderDrawColor(app->renderer, 255, 51, 0, 180);
    }
    SDL_RenderFillRect(app->renderer, &goldPanel);
    // for icon and text loading
    SDL_RenderRect(app->renderer, &goldPanel);
    SDL_RenderTexture(app->renderer, app->goldTexture, NULL, &icon);
    drawText(app, goldAmmount, textPos);
}

void drawEndScreen(AppState *app){
    char winner[62];
    snprintf(winner, sizeof(winner), "PLAYER %d VICTORY!!!", app->winner);
    SDL_FRect textPos = {
        900.0f,                 // X padding from left
        (WINDOW_HEIGHT / 2) - 100, // Y position inside panel
        120.0f,                 // Width
        120.0f                  // Height
    };
    drawText(app, winner, textPos);
}

void quitPauseButton(AppState *app){
    float wB, hB;
    SDL_GetTextureSize(app->buttonHovered, &wB, &hB);

    app->quitbutton.w = wB;
    app->quitbutton.h = hB;
    app->quitbutton.x = (float)(WINDOW_WIDTH / 2) - (app->quitbutton.w/2);
    app->quitbutton.y = 500.0f;

    SDL_RenderTexture(app->renderer, app->buttonHovered, NULL, &app->quitbutton);
    drawText(app, "QUIT", app->quitbutton);
}

void mainMenuButton(AppState *app){
    float wB, hB;
    SDL_GetTextureSize(app->buttonHovered, &wB, &hB);

    app->mainmenubutton.w = wB;
    app->mainmenubutton.h = hB;
    app->mainmenubutton.x = (float)(WINDOW_WIDTH / 2) - (app->mainmenubutton.w/2);
    app->mainmenubutton.y = 300.0f;

    SDL_RenderTexture(app->renderer, app->buttonHovered, NULL, &app->mainmenubutton);
    drawText(app, "MAIN MENU", app->mainmenubutton);
}

void optionsButton(AppState *app){
    float wB, hB;
    SDL_GetTextureSize(app->buttonHovered, &wB, &hB);

    app->optionsbutton.w = wB;
    app->optionsbutton.h = hB;
    app->optionsbutton.x = (float)(WINDOW_WIDTH / 2) - (app->optionsbutton.w/2);
    app->optionsbutton.y = 400.0f;

    SDL_RenderTexture(app->renderer, app->buttonHovered, NULL, &app->optionsbutton);
    drawText(app, "OPTIONS", app->optionsbutton);
}

void techTreeButton(AppState *app){
    float wB, hB;
    SDL_GetTextureSize(app->buttonHovered, &wB, &hB);

    app->techTreeButton.w = wB;
    app->techTreeButton.h = hB;
    app->techTreeButton.x = 150.0f;
    app->techTreeButton.y = WINDOW_HEIGHT - app->techTreeButton.h - 20.0f;

    SDL_RenderTexture(app->renderer, app->buttonHovered, NULL, &app->techTreeButton);
    drawText(app, "TECH TREE", app->techTreeButton);    
}

void playerRectangles(AppState *app){

    float panelW = 300.0f;
    float panelH = 400.0f;
    float margin = 20.0f;
    float spacing = 30.0f;

    //p1 rectangle setup (Right)
    app->p1Rect.x = WINDOW_WIDTH - panelW - margin;
    app->p1Rect.y = margin;
    app->p1Rect.h = panelH;
    app->p1Rect.w = panelW;

    //p2 rectangle setup (left)
    app->p2Rect.x = margin;
    app->p2Rect.y = margin;
    app->p2Rect.h = panelH;
    app->p2Rect.w = panelW;

    SDL_FRect rows[8];
    for(int i = 0; i<8; i++){
        rows[i] = (SDL_FRect){app->p1Rect.x, app->p1Rect.y + 10 + (i * spacing), app->p1Rect.w, spacing};
    }
    SDL_SetRenderDrawBlendMode(app->renderer, SDL_BLENDMODE_BLEND);

    //player1
    SDL_SetRenderDrawColor(app->renderer, 0, 153, 255, 180);
    SDL_RenderFillRect(app->renderer, &app->p1Rect);
    SDL_RenderRect(app->renderer, &app->p1Rect);
    drawText(app, "PLAYER 1", rows[0]);

    //player 2
    SDL_SetRenderDrawColor(app->renderer, 255, 51, 0, 180);
    SDL_RenderFillRect(app->renderer, &app->p2Rect);
    SDL_RenderRect(app->renderer, &app->p2Rect);

    char p2Stats[128];
    snprintf(p2Stats, sizeof(p2Stats), "PLAYER 2\nTroops: %d\nTowns: %d", app->P2.pieceCount, app->P2.towns);
    drawText(app, p2Stats, app->p2Rect);

}