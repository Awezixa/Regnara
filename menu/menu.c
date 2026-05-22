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

UIState getPieceUIState(AppState *app, pieceType type)
{
    if (!pieceUnlocked(app, type))
        return UI_LOCKED;

    if (pieceCountReached(app, type))
        return UI_UNAVAILABLE;

    return UI_AVAILABLE;
}

void unitIconUI(AppState *app)
{
    SDL_FPoint mousePoint = {app->input.mouseX, app->input.mouseY};

    UnitUIButton units[] =
    {
        { PAWN, ENVOY, &app->pawnButton,
          app->UIBluePawnAvailable, app->UIBluePawnUnavailable, app->UIBluePawnLocked,
          app->UIRedPawnAvailable, app->UIRedPawnUnavailable, app->UIRedPawnLocked },

        { KNIGHT, LANCER, &app->knightButton,
          app->UIBlueKnightAvailable, app->UIBlueKnightUnavailable, app->UIBlueKnightLocked,
          app->UIRedKnightAvailable, app->UIRedKnightUnavailable, app->UIRedKnightLocked },

        { BISHOP, MAGE, &app->bishopButton,
          app->UIBlueBishopAvailable, app->UIBlueBishopUnavailable, app->UIBlueBishopLocked,
          app->UIRedBishopAvailable, app->UIRedBishopUnavailable, app->UIRedBishopLocked },

        { ROOK, CATAPULT, &app->rookButton,
          app->UIBlueRookAvailable, app->UIBlueRookUnavailable, app->UIBlueRookLocked,
          app->UIRedRookAvailable, app->UIRedRookUnavailable, app->UIRedRookLocked },

        { QUEEN, QUEEN, &app->queenButton,
          app->UIBlueQueenAvailable, app->UIBlueQueenUnavailable, app->UIBlueQueenLocked,
          app->UIRedQueenAvailable, app->UIRedQueenUnavailable, app->UIRedQueenLocked },
    };

    int unitCount = sizeof(units) / sizeof(units[0]);

    float spacing = 120.0f;
    float startX = (WINDOW_WIDTH / 2.0f) - (((unitCount - 1) * spacing) / 2.0f);

    float sizex = 96.0f;
    float sizey = 144.0f;

    // ----------------------------
    // 1. POSITION + DRAW UNIT ICONS
    // ----------------------------
    for (int i = 0; i < unitCount; i++)
    {
        units[i].button->x = startX + (i * spacing);
        units[i].button->y = WINDOW_HEIGHT - 160.0f;
        units[i].button->w = sizex;
        units[i].button->h = sizey;

        UIState state = getPieceUIState(app, units[i].baseType);

        SDL_Texture *tex = NULL;

        if (app->currentPlayer == 1)
        {
            if (state == UI_AVAILABLE) tex = units[i].blueAvailable;
            else if (state == UI_UNAVAILABLE) tex = units[i].blueUnavailable;
            else tex = units[i].blueLocked;
        }
        else
        {
            if (state == UI_AVAILABLE) tex = units[i].redAvailable;
            else if (state == UI_UNAVAILABLE) tex = units[i].redUnavailable;
            else tex = units[i].redLocked;
        }

        SDL_RenderTexture(app->renderer, tex, NULL, units[i].button);

        // highlight selected type
        if (app->selectedPieceType == units[i].baseType && state == UI_AVAILABLE)
        {
            SDL_Texture *overlay =
                (app->currentPlayer == 1)
                ? app->UIBlueOverlayAvailable
                : app->UIRedOverlayAvailable;

            SDL_FRect overlayRect = {
                units[i].button->x,
                units[i].button->y - 33.0f,
                96,
                36
            };

            SDL_RenderTexture(app->renderer, overlay, NULL, &overlayRect);
        }

    }

    // ----------------------------
    // 2. CLICK SELECTION
    // ----------------------------
    if (app->input.mouseLeftPressed)
    {
        for (int i = 0; i < unitCount; i++)
        {
            if (SDL_PointInRectFloat(&mousePoint, units[i].button))
            {
                app->selectedPieceType = units[i].baseType;
            }
        }
    }

    // ----------------------------
    // 3. UPGRADE PLATFORM BUTTON (7)
    // ----------------------------

    if (app->selectedPiece && isSelectedPieceOnUpgradeTile(app))
    {
        TechTree *tree =
            (app->currentPlayer == 1)
            ? &app->techTreeP1
            : &app->techTreeP2;

        float upgradePlatformX =
            startX + (unitCount * spacing) + spacing;

        SDL_FRect platformButton = {
            upgradePlatformX,
            WINDOW_HEIGHT - 160.0f,
            sizex,
            sizey
        };

        UIState state =
            isUpgradePlatformUnlocked(tree)
            ? UI_AVAILABLE
            : UI_LOCKED;

        SDL_Texture *tex =
            (app->currentPlayer == 1)
                ? (state == UI_AVAILABLE
                    ? app->UIBlueUpgPlatAvailable
                    : app->UIBlueUpgPlatLocked)
                : (state == UI_AVAILABLE
                    ? app->UIRedUpgPlatAvailable
                    : app->UIRedUpgPlatLocked);

        SDL_RenderTexture(app->renderer, tex, NULL, &platformButton);

        // click handling (placeholder for now)
        if (app->input.mouseLeftPressed)
        {
            SDL_FPoint mouse = { app->input.mouseX, app->input.mouseY };

            if (SDL_PointInRectFloat(&mouse, &platformButton))
            {
                if (state == UI_AVAILABLE)
                {
                    printf("Upgrade Platform clicked!\n");
                }
            }
        }
    }

    // ----------------------------
    // 4. UPGRADE BUTTON
    // ----------------------------

    if (app->selectedPiece != NULL)
    {
        pieceType upgradeType =
            getUpgradeType(app->selectedPiece->type);

        if (upgradeType != KING)
        {
        SDL_FRect upgradeButton = {
            startX + ((unitCount - 1) * spacing) + spacing,
            WINDOW_HEIGHT - 160.0f,
            sizex,
            sizey
        };

        printf("Selected type: %d\n", app->selectedPieceType);
        printf("Upgrade type: %d\n", getUpgradeType(app->selectedPieceType));

        UIState state = getPieceUIState(app, upgradeType);
        SDL_Texture *tex = getPieceTexture(app, upgradeType, state);

        SDL_RenderTexture(app->renderer, tex, NULL, &upgradeButton);

        if (app->input.mouseLeftPressed &&
            SDL_PointInRectFloat(&mousePoint, &upgradeButton))
            {
                if (state == UI_AVAILABLE)
                {
                    app->selectedPiece->type = upgradeType;

                    // optional gold cost
                    int cost = pieceCost(upgradeType);

                    if (app->currentPlayer == 1)
                        app->P1.p1Gold -= cost;
                    else
                        app->P2.p2Gold -= cost;

                    app->selectedPiece = NULL;
                }
            }
    }
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
    SDL_RenderTexture(app->renderer, app->blueInfoBox, NULL, &panel);}
    else{
    SDL_RenderTexture(app->renderer, app->redInfoBox, NULL, &panel);}
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
    snprintf(townStr, sizeof(townStr), "%d", towns);

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

SDL_Texture *getPieceTexture(AppState *app, pieceType type, UIState state)
{
    // Decide player-based color set first
    int isPlayer1 = (app->currentPlayer == 1);

    switch (type)
    {
        case PAWN:
            if (isPlayer1)
            {
                if (state == UI_AVAILABLE) return app->UIBluePawnAvailable;
                if (state == UI_UNAVAILABLE) return app->UIBluePawnUnavailable;
                return app->UIBluePawnLocked;
            }
            else
            {
                if (state == UI_AVAILABLE) return app->UIRedPawnAvailable;
                if (state == UI_UNAVAILABLE) return app->UIRedPawnUnavailable;
                return app->UIRedPawnLocked;
            }

        case KNIGHT:
            if (isPlayer1)
            {
                if (state == UI_AVAILABLE) return app->UIBlueKnightAvailable;
                if (state == UI_UNAVAILABLE) return app->UIBlueKnightUnavailable;
                return app->UIBlueKnightLocked;
            }
            else
            {
                if (state == UI_AVAILABLE) return app->UIRedKnightAvailable;
                if (state == UI_UNAVAILABLE) return app->UIRedKnightUnavailable;
                return app->UIRedKnightLocked;
            }

        case BISHOP:
            if (isPlayer1)
            {
                if (state == UI_AVAILABLE) return app->UIBlueBishopAvailable;
                if (state == UI_UNAVAILABLE) return app->UIBlueBishopUnavailable;
                return app->UIBlueBishopLocked;
            }
            else
            {
                if (state == UI_AVAILABLE) return app->UIRedBishopAvailable;
                if (state == UI_UNAVAILABLE) return app->UIRedBishopUnavailable;
                return app->UIRedBishopLocked;
            }

        case ROOK:
            if (isPlayer1)
            {
                if (state == UI_AVAILABLE) return app->UIBlueRookAvailable;
                if (state == UI_UNAVAILABLE) return app->UIBlueRookUnavailable;
                return app->UIBlueRookLocked;
            }
            else
            {
                if (state == UI_AVAILABLE) return app->UIRedRookAvailable;
                if (state == UI_UNAVAILABLE) return app->UIRedRookUnavailable;
                return app->UIRedRookLocked;
            }

        case QUEEN:
            if (isPlayer1)
            {
                if (state == UI_AVAILABLE) return app->UIBlueQueenAvailable;
                if (state == UI_UNAVAILABLE) return app->UIBlueQueenUnavailable;
                return app->UIBlueQueenLocked;
            }
            else
            {
                if (state == UI_AVAILABLE) return app->UIRedQueenAvailable;
                if (state == UI_UNAVAILABLE) return app->UIRedQueenUnavailable;
                return app->UIRedQueenLocked;
            }
        
        case ENVOY:
            if (isPlayer1)
            {
                if (state == UI_AVAILABLE) return app->UIBlueEnvoyAvailable;
                if (state == UI_UNAVAILABLE) return app->UIBlueEnvoyUnavailable;
                return app->UIBlueEnvoyLocked;
            }
            else
            {
                if (state == UI_AVAILABLE) return app->UIRedEnvoyAvailable;
                if (state == UI_UNAVAILABLE) return app->UIRedEnvoyUnavailable;
                return app->UIRedEnvoyLocked;
            }

        case LANCER:
            if (isPlayer1)
            {
                if (state == UI_AVAILABLE) return app->UIBlueLancerAvailable;
                if (state == UI_UNAVAILABLE) return app->UIBlueLancerUnavailable;
                return app->UIBlueLancerLocked;
            }
            else
            {
                if (state == UI_AVAILABLE) return app->UIRedLancerAvailable;
                if (state == UI_UNAVAILABLE) return app->UIRedLancerUnavailable;
                return app->UIRedLancerLocked;
            }

        case MAGE:
            if (isPlayer1)
            {
                if (state == UI_AVAILABLE) return app->UIBlueMageAvailable;
                if (state == UI_UNAVAILABLE) return app->UIBlueMageUnavailable;
                return app->UIBlueMageLocked;
            }
            else
            {
                if (state == UI_AVAILABLE) return app->UIRedMageAvailable;
                if (state == UI_UNAVAILABLE) return app->UIRedMageUnavailable;
                return app->UIRedMageLocked;
            }

        case CATAPULT:
            if (isPlayer1)
            {
                if (state == UI_AVAILABLE) return app->UIBlueCatapultAvailable;
                if (state == UI_UNAVAILABLE) return app->UIBlueCatapultUnavailable;
                return app->UIBlueCatapultLocked;
            }
            else
            {
                if (state == UI_AVAILABLE) return app->UIRedCatapultAvailable;
                if (state == UI_UNAVAILABLE) return app->UIRedCatapultUnavailable;
                return app->UIRedCatapultLocked;
            }
        default:
            return NULL;
    }
}

void drawOptions(AppState *app){

    app->optText.x = (float)((WINDOW_WIDTH)/2.0f) - 50.0f;
    app->optText.y = 150.0f;
    app->optText.w = 100.0f;
    app->optText.h = 50.0f;    


    float imgW, imgH;

    SDL_GetTextureSize(app->controlsTexture, &imgW, &imgH);

    float maxWidth = 1200.0f;
    float maxHeight = 800.0f;

    float scale = imgW / imgH;
    
    app->controlsRect.w = maxWidth;
    app->controlsRect.h = maxWidth / scale;

    
    app->controlsRect.x = (float)(WINDOW_WIDTH - app->controlsRect.w) / 2.0f;
    app->controlsRect.y = (float)(WINDOW_HEIGHT - app->controlsRect.h) / 2.0f;

    drawText(app, app->font, "OPTIONS", app->optText);
    SDL_RenderTexture(app->renderer, app->controlsTexture, NULL, &app->controlsRect);
}


void drawTextWrapped(AppState *app, TTF_Font *font, const char *text, SDL_FRect container, int wrapLengthPixels)
{
    SDL_Color white = {255, 255, 255, 255};

    // 1. Render text with SDL3_ttf wrapping logic enabled
    SDL_Surface *surface = TTF_RenderText_Blended_Wrapped(font, text, 0, white, wrapLengthPixels);
    if (!surface) return;

    SDL_Texture *texture = SDL_CreateTextureFromSurface(app->renderer, surface);
    SDL_DestroySurface(surface);
    if (!texture) return;

    float textW, textH;
    SDL_GetTextureSize(texture, &textW, &textH);

    // 2. Position text box at top-left inside container with margins (instead of full centering)
    SDL_FRect textRect = {
        container.x,
        container.y,
        textW,
        textH
    };

    SDL_RenderTexture(app->renderer, texture, NULL, &textRect);
    SDL_DestroyTexture(texture);
}