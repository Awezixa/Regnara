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
    drawText(app, app->font, "PLAY", app->playbutton);
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
    drawText(app, app->font, "QUIT", app->quitbutton);
}

// Xavier
void drawText(AppState *app, TTF_Font *font, const char *text, SDL_FRect container)
{
    SDL_Color black = {255, 255, 255, 255}; // can change colour if needed

    // 1. Create a surface from the font
    SDL_Surface *surface = TTF_RenderText_Blended(font, text, 0, black);
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
    drawText(app, app->font, "END TURN", app->endTurnButton);
}

void turnUI(AppState *app)
{
    char turnStr[100];
    snprintf(turnStr, sizeof(turnStr), "| Player %d || Turn %d |", app->currentPlayer, app->turnCounter);

    app->turnCount.x = (float)(WINDOW_WIDTH / 2) - 50.0f;
    app->turnCount.y = 20.0f;
    app->turnCount.h = 60.0f;
    app->turnCount.w = 200.0f;

    drawText(app, app->font, turnStr, app->turnCount);
}

void drawPauseMenu(AppState *app)
{
    char pauseUI[32];
    snprintf(pauseUI, sizeof(pauseUI), "PAUSED");

    app->pauseTxt.h = 50.0f;
    app->pauseTxt.w = 180.0f;
    app->pauseTxt.x = (float)(WINDOW_WIDTH / 2) - (app->pauseTxt.w/2);
    app->pauseTxt.y = 200.0f;

    drawText(app, app->font, pauseUI, app->pauseTxt);
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
    //buildingIconUI(app);
    techTreeButton(app);
    endTurnButton(app);
}

void unitIconUI(AppState *app)
{
    pieceType types[] = {PAWN, KNIGHT, BISHOP, ROOK, QUEEN, ENVOY, MAGE, CATAPAULT, LANCER};
    SDL_Texture* texturesP1[] = {app->bluePawnTexture, app->blueKnightTexture, app->blueBishopTexture, app->blueRookTexture, app->blueQueenTexture, app->blueEnvoyTexture, app->blueMageTexture, app->blueCatapaultTexture, app->blueLancerTexture};
    SDL_Texture* texturesP2[] = {app->redPawnTexture, app->redKnightTexture, app->redBihsopTexture, app->redRookTexture, app->redQueenTexture, app->redEnvoyTexture, app->redMageTexture, app->redCatapaultTexture, app->redLancerTexture};
    SDL_FRect* buttons[] = {&app->pawnButton, &app->knightButton, &app->bishopButton, &app->rookButton, &app->queenButton, &app->envoyButton, &app->mageButton, &app->catapaultButton, &app->lancerButton};
    
    float spacing = 90.0f;
    float startX = (WINDOW_WIDTH / 2.0f) - ((spacing * 5) / 2.0f);
    float size = 64.0f;
    

    for (int i = 0; i < 9; i++)
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
        drawText(app, app->font, cost, costPos);
    }
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
    mainMenuButton(app);
    quitButton(app);
    drawText(app, app->font, winner, textPos);
}

void quitPauseButton(AppState *app){
    float wB, hB;
    SDL_GetTextureSize(app->buttonHovered, &wB, &hB);

    app->quitbutton.w = wB;
    app->quitbutton.h = hB;
    app->quitbutton.x = (float)(WINDOW_WIDTH / 2) - (app->quitbutton.w/2);
    app->quitbutton.y = 500.0f;

    SDL_RenderTexture(app->renderer, app->buttonHovered, NULL, &app->quitbutton);
    drawText(app, app->font, "QUIT", app->quitbutton);
}

void mainMenuButton(AppState *app){
    float wB, hB;
    SDL_GetTextureSize(app->buttonHovered, &wB, &hB);

    app->mainmenubutton.w = wB;
    app->mainmenubutton.h = hB;
    app->mainmenubutton.x = (float)(WINDOW_WIDTH / 2) - (app->mainmenubutton.w/2);
    app->mainmenubutton.y = 300.0f;

    SDL_RenderTexture(app->renderer, app->buttonHovered, NULL, &app->mainmenubutton);
    drawText(app, app->font, "MAIN MENU", app->mainmenubutton);
}

void optionsButton(AppState *app){
    float wB, hB;
    SDL_GetTextureSize(app->buttonHovered, &wB, &hB);

    app->optionsbutton.w = wB;
    app->optionsbutton.h = hB;
    app->optionsbutton.x = (float)(WINDOW_WIDTH / 2) - (app->optionsbutton.w/2);
    app->optionsbutton.y = 400.0f;

    SDL_RenderTexture(app->renderer, app->buttonHovered, NULL, &app->optionsbutton);
    drawText(app, app->font, "OPTIONS", app->optionsbutton);
}

void techTreeButton(AppState *app){
    float wB, hB;
    SDL_GetTextureSize(app->buttonHovered, &wB, &hB);

    app->techTreeButton.w = wB;
    app->techTreeButton.h = hB;
    app->techTreeButton.x = 150.0f;
    app->techTreeButton.y = WINDOW_HEIGHT - app->techTreeButton.h - 20.0f;

    SDL_RenderTexture(app->renderer, app->buttonHovered, NULL, &app->techTreeButton);
    drawText(app, app->font, "TECH TREE", app->techTreeButton);    
}

void playerRectangles(AppState *app) {
    float panelW = 400.0f;
    float panelH = 128.0f;
    float margin = 20.0f;

    // Player 1 (Blue - Left side)
    SDL_FRect p1GoldRect = { 0, margin, panelW, panelH };

    // Player 2 (Red - Right side)
    SDL_FRect p2GoldRect = { WINDOW_WIDTH - panelW, margin, panelW, panelH };

    // Draw both using the helper
    drawSinglePlayerStatus(app, p1GoldRect, "PLAYER 1", 1);
    drawSinglePlayerStatus(app, p2GoldRect, "PLAYER 2", 2);
}

void drawSinglePlayerStatus(AppState *app, SDL_FRect panel, const char* title, int playerNum) {
    // 1. Draw Background and Outline
    SDL_SetRenderDrawBlendMode(app->renderer, SDL_BLENDMODE_BLEND);
    if(playerNum == 1){
    SDL_RenderTexture(app->renderer, app->blueGoldTexture, NULL, &panel);}
    else{
    SDL_RenderTexture(app->renderer, app->redGoldTexture, NULL, &panel);}
    SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, 255);
    SDL_RenderRect(app->renderer, &panel);

    // 2. Define the vertical rows
    float spacing = 40.0f; 
    SDL_FRect rows[8];
    for(int i = 0; i < 8; i++) {
        rows[i] = (SDL_FRect){ panel.x, panel.y + 10 + (i * spacing), panel.w, spacing };
    }

    // 3. Render the Title (Line 0)
    drawText(app, app->font, title, rows[0]);

    // 4. Render Values
    int gold = (playerNum == 1) ? app->P1.p1Gold : app->P2.p2Gold;
    int towns = (playerNum == 1) ? app->P1.towns : app->P2.towns;

    char goldStr[32];
    char townStr[32];
    char troopStr[32];

    snprintf(goldStr, sizeof(goldStr), "%d", gold);
    snprintf(townStr, sizeof(townStr), "%d", towns+1);

    SDL_FRect goldRect = {
    panel.x + 70.0f,
    panel.y + 70.0f,
    120.0f,
    30.0f
    };

    SDL_FRect townRect = {
        panel.x + 254.0f,
        panel.y + 70.0f,
        120.0f,
        30.0f
    };

    drawText(app, app->fontLarge, townStr, townRect);
    drawText(app, app->fontLarge, goldStr, goldRect);
}