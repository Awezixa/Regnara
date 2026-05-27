#include "../game.h"
#include "menu.h"

// move to Menu. Xavier
void drawMainMenu(AppState *app)
{
    float w, h;

    SDL_GetTextureSize(app->mainMenuBackground, &w, &h);

    SDL_FRect bg = {0, 0, w, h};
    SDL_RenderTexture(app->renderer, app->mainMenuBackground, NULL, &bg);

    SDL_GetTextureSize(app->logoTexture, &w, &h);

    SDL_FRect dst = {(float)((WINDOW_WIDTH / 2) - ((w) / 2)-20), 10, (w), (h)}; // to make logo readable and centered on screen
    SDL_RenderTexture(app->renderer, app->logoTexture, NULL, &dst);

    playButton(app); // play button
    optionsButton(app);
    quitButton(app);
}

//main menu buttons
void playButton(AppState *app){

    drawButton(app, &app->playbutton, "PLAY", (float)((WINDOW_WIDTH / 2) -20) - (app->playbutton.w/2), 500.0f);
}

void quitButton(AppState *app){

    drawButton(app, &app->quitbutton, "QUIT", (float)((WINDOW_WIDTH / 2) -20) - (app->playbutton.w/2), 700.0f);
}
void optionsButton(AppState *app){

    drawButton(app, &app->optionsbutton, "OPTIONS", (float)((WINDOW_WIDTH / 2) -20) - (app->optionsbutton.w/2), 600.0f);
}

void optionsBackButton(AppState *app)
{
    drawBackButton(app, &app->optionsBackButton, "X", 45, 45);
}

void techTreeBackButton(AppState *app)
{
    drawBackButton(app, &app->techTreeBackButton, "X", 45, 45);
}


//other ui Helpers

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

void drawButton(AppState *app, SDL_FRect *buttonRect, const char *text, float x, float y){
    SDL_FPoint mousePoint = {
        app->input.mouseX,
        app->input.mouseY
    };

    // Set position first
    buttonRect->x = x;
    buttonRect->y = y;

    // Detect hover
    bool hovered =
        SDL_PointInRectFloat(&mousePoint, buttonRect);

    // Pick texture
    SDL_Texture *buttonTex =
        hovered ? app->buttonOn : app->buttonOff;

    // Get texture size
    float w, h;
    SDL_GetTextureSize(buttonTex, &w, &h);

    buttonRect->w = w;
    buttonRect->h = h;

    // Re-check hover now that size exists
    hovered =
        SDL_PointInRectFloat(&mousePoint, buttonRect);

    // Re-pick texture
    buttonTex =
        hovered ? app->buttonOn : app->buttonOff;

    // Draw button
    SDL_RenderTexture(app->renderer,
                      buttonTex,
                      NULL,
                      buttonRect);

    // Text color
    SDL_Color textColor;

    if (hovered)
    {
        textColor = (SDL_Color){0, 0, 0, 255};
    }
    else
    {
        textColor = (SDL_Color){255, 255, 255, 255};
    }

    // Create text surface
    SDL_Surface *surface =
        TTF_RenderText_Blended(app->font,
                               text,
                               0,
                               textColor);

    if (!surface)
        return;

    // Create text texture
    SDL_Texture *textTexture =
        SDL_CreateTextureFromSurface(app->renderer,
                                     surface);

    SDL_DestroySurface(surface);

    if (!textTexture)
        return;

    // Center text
    float textW, textH;
    SDL_GetTextureSize(textTexture, &textW, &textH);

    SDL_FRect textRect = {
        buttonRect->x + (buttonRect->w - textW) / 2.0f,
        buttonRect->y + (buttonRect->h - textH) / 2.0f,
        textW,
        textH
    };

    // Draw text
    SDL_RenderTexture(app->renderer,
                      textTexture,
                      NULL,
                      &textRect);

    SDL_DestroyTexture(textTexture);
}

void drawBackButton(AppState *app, SDL_FRect *buttonRect, const char *text, float x, float y){
    SDL_FPoint mousePoint = {
        app->input.mouseX,
        app->input.mouseY
    };

    // Set position first
    buttonRect->x = x;
    buttonRect->y = y;

    // Detect hover
    bool hovered =
        SDL_PointInRectFloat(&mousePoint, buttonRect);

    // Pick texture
    SDL_Texture *buttonTex =
        hovered ? app->backButtonOn : app->backButtonOff;

    buttonRect->w = 80;
    buttonRect->h = 80;

    // Re-check hover now that size exists
    hovered =
        SDL_PointInRectFloat(&mousePoint, buttonRect);

    // Re-pick texture
    buttonTex =
        hovered ? app->backButtonOn : app->backButtonOff;

    // Draw button
    SDL_RenderTexture(app->renderer,
                      buttonTex,
                      NULL,
                      buttonRect);

    // Text color
    SDL_Color textColor;

    if (hovered)
    {
        textColor = (SDL_Color){0, 0, 0, 255};
    }
    else
    {
        textColor = (SDL_Color){255, 255, 255, 255};
    }

    // Create text surface
    SDL_Surface *surface =
        TTF_RenderText_Blended(app->font,
                               text,
                               0,
                               textColor);

    if (!surface)
        return;

    // Create text texture
    SDL_Texture *textTexture =
        SDL_CreateTextureFromSurface(app->renderer,
                                     surface);

    SDL_DestroySurface(surface);

    if (!textTexture)
        return;

    // Center text
    float textW, textH;
    SDL_GetTextureSize(textTexture, &textW, &textH);

    SDL_FRect textRect = {
        buttonRect->x + (buttonRect->w - textW) / 2.0f,
        buttonRect->y + (buttonRect->h - textH) / 2.0f,
        textW,
        textH
    };

    // Draw text
    SDL_RenderTexture(app->renderer,
                      textTexture,
                      NULL,
                      &textRect);

    SDL_DestroyTexture(textTexture);
}


void endTurnButton(AppState *app)
{
    float scale = 1.0f;

    drawButton(app, &app->endTurnButton, "END TURN", WINDOW_WIDTH - app->endTurnButton.w - 50.0f, WINDOW_HEIGHT - app->endTurnButton.h - 20.0f);
}

void turnUI(AppState *app)
{
    char turnStr[100];
    snprintf(turnStr, sizeof(turnStr), "| Player %d || Turn %d |", app->currentPlayer, app->turnCounter);

    app->turnCount.x = (float)(WINDOW_WIDTH / 2) - 113;
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
    // add resume button
    quitPauseButton(app);
    mainMenuButton(app);
    pauseOptionsButton(app);
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

UIState getPieceUIState(AppState *app, PieceType type)
{
    if (!pieceUnlocked(app, type))
        return UI_LOCKED;

    if (isPieceAtCap(app, app->currentPlayer, type))
        return UI_UNAVAILABLE;

    int cost = pieceCost(type);
    int currentGold = (app->currentPlayer == 1) ? app->P1.p1Gold : app->P2.p2Gold;
    
    if (currentGold < cost)
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
    float startX = (WINDOW_WIDTH / 2.0f) - (((unitCount - 1) * spacing) / 2.0f) - 48;

    float sizex = 96.0f;
    float sizey = 144.0f;


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

        char pieceCap[64];

int pieceCount =
    getFamilyCount  (app,
                    app->currentPlayer,
                    units[i].baseType);

int maxPieces =
    getMaxPiecesForType(app,
                        app->currentPlayer,
                        units[i].baseType);

snprintf(pieceCap,
         sizeof(pieceCap),
         "%d/%d",
         pieceCount,
         maxPieces);

        SDL_FRect countTextRect = {
    units[i].button->x,
    units[i].button->y - 30.0f,
    96.0f,
    36.0f};


        if (app->selectedPieceType == units[i].baseType)
        {
            SDL_Texture *overlay = NULL;

            if (state == UI_AVAILABLE)
            {
                overlay =
                    (app->currentPlayer == 1)
                    ? app->UIBlueOverlayAvailable
                    : app->UIRedOverlayAvailable;
            }
            else if (state == UI_UNAVAILABLE)
            {
                overlay =
                    (app->currentPlayer == 1)
                    ? app->UIBlueOverlayUnavailable
                    : app->UIRedOverlayUnavailable;
            }

            if (overlay != NULL)
            {
                SDL_FRect overlayRect = {
                    units[i].button->x,
                    units[i].button->y - 33.0f,
                    96,
                    36
                };

                SDL_RenderTexture(app->renderer, overlay, NULL, &overlayRect);
                drawText(app, app->font, pieceCap, countTextRect);
            }
        }

    }


    if (app->input.mouseLeftPressed)
    {
        for (int i = 0; i < unitCount; i++)
        {
            if (SDL_PointInRectFloat(&mousePoint, units[i].button))
            {
                UIState state = getPieceUIState(app, units[i].baseType);

                if (state == UI_AVAILABLE)
                {
                    app->selectedPieceType = units[i].baseType;
                }
            }
        }
    }


    if (app->selectedPiece &&
    isSelectedPieceOnUpgradeTile(app))
    {
        TechTree *tree =
            (app->currentPlayer == 1)
            ? &app->techTreeP1
            : &app->techTreeP2;

        float upgradePlatformX =
            startX - spacing-20;

        SDL_FRect platformButton = {
            upgradePlatformX,
            WINDOW_HEIGHT - 160.0f,
            sizex,
            sizey
        };

        UIState state;

        if (!isUpgradePlatformUnlocked(tree))
        {
            state = UI_LOCKED;
        }
        else if (app->selectedPiece->abilityUsed)
        {
            state = UI_UNAVAILABLE;
        }
        else
        {
            state = UI_AVAILABLE;
        }

        SDL_Texture *tex = NULL;

        if (app->currentPlayer == 1)
        {
            if (state == UI_AVAILABLE)
                tex = app->UIBlueUpgPlatAvailable;
            else if (state == UI_UNAVAILABLE)
                tex = app->UIBlueUpgPlatUnavailable;
            else
                tex = app->UIBlueUpgPlatLocked;
        }
        else
        {
            if (state == UI_AVAILABLE)
                tex = app->UIRedUpgPlatAvailable;
            else if (state == UI_UNAVAILABLE)
                tex = app->UIRedUpgPlatUnavailable;
            else
                tex = app->UIRedUpgPlatLocked;
        }

        if(app->selectedPiece->type != KING)
        SDL_RenderTexture(app->renderer, tex, NULL, &platformButton);

        // click handling (placeholder for now)
        if (app->input.mouseLeftPressed)
        {
            SDL_FPoint mouse = { app->input.mouseX, app->input.mouseY };

            if (SDL_PointInRectFloat(&mouse, &platformButton))
            {
                if (state == UI_AVAILABLE)
                {
                    useUpgradePlatform(app, app->selectedPiece);
                }
            }
        }
    }


    if (app->selectedPiece != NULL)
    {
        PieceType upgradeType =
            getUpgradeType(app->selectedPiece->type);

        if (upgradeType != KING)
        {
        SDL_FRect upgradeButton = {
            startX + ((unitCount - 1) * spacing) + spacing + 20,
            WINDOW_HEIGHT - 160.0f,
            sizex,
            sizey
        };

        printf("Selected type: %d\n", app->selectedPieceType);
        printf("Upgrade type: %d\n", getUpgradeType(app->selectedPieceType));

        UIState state = getPieceUIState(app, upgradeType);
        SDL_Texture *tex = getPieceTexture(app, upgradeType, state);

        SDL_RenderTexture(app->renderer, tex, NULL, &upgradeButton);

        if (app->input.mouseLeftPressed && SDL_PointInRectFloat(&mousePoint, &upgradeButton))
            {
                if (state == UI_AVAILABLE)
                {
                    app->selectedPiece->type = upgradeType;
                    app->selectedPiece->moved = true;

                    // optional gold cost
                    int cost = pieceCost(upgradeType);

                    if (app->currentPlayer == 1)
                        app->P1.p1Gold -= cost;
                    else
                        app->P2.p2Gold -= cost;

                    app->lastInteractedPiece = app->selectedPiece;

                    // FIXED: Reset selection values to prevent overlapping check loops
                    app->selectedPieceType = KING; 
                    app->selectedPiece = NULL;
                }
            }
        }
    }
}


void drawEndScreen(AppState *app){
    char winner[62];

    float w, h;

    SDL_GetTextureSize(app->mainMenuBackground, &w, &h);

    SDL_FRect bg = {0, 0, w, h};

    if(app->winner == 1)
        SDL_RenderTexture(app->renderer, app->winner1, NULL, &bg);
    else
        SDL_RenderTexture(app->renderer, app->winner2, NULL, &bg);


    snprintf(winner, sizeof(winner), "PLAYER %d VICTORY!!!", app->winner);
    SDL_FRect textPos = {
        900.0f,                 // X padding from left
        (WINDOW_HEIGHT / 2) - 100, // Y position inside panel
        120.0f,                 // Width
        120.0f                  // Height
    };
    endMainMenuButton(app);
    endQuitButton(app);
    drawText(app, app->font, winner, textPos);
}

// pause UI
void quitPauseButton(AppState *app){

    drawButton(app, &app->quitbutton, "QUIT", (float)(WINDOW_WIDTH / 2) - (app->quitbutton.w/2), 500.0f);
}

void mainMenuButton(AppState *app){

    drawButton(app, &app->mainmenubutton, "MAIN MENU", (float)(WINDOW_WIDTH / 2) - (app->mainmenubutton.w/2), 300.0f);
}

void pauseOptionsButton(AppState *app){
    drawButton(app, &app->optionsbutton, "OPTIONS", (float)(WINDOW_WIDTH / 2) - (app->optionsbutton.w/2), 400.0f);
}

void drawOptions(AppState *app){

    app->optText.x = (float)((WINDOW_WIDTH)/2.0f) - 50.0f;
    app->optText.y = 150.0f;
    app->optText.w = 100.0f;
    app->optText.h = 50.0f;    

    float imgW, imgH;

    SDL_GetTextureSize(app->controlsTexture, &imgW, &imgH);

    float maxWidth = 1920.0f;
    float maxHeight = 1080.0f;

    float scale = imgW / imgH;
    
    app->controlsRect.w = maxWidth;
    app->controlsRect.h = maxWidth / scale;

    
    app->controlsRect.x = 0;
    app->controlsRect.y = 0;
    SDL_RenderTexture(app->renderer, app->controlsTexture, NULL, &app->controlsRect);
    volumeControls(app);
}


bool drawSlider(AppState *app, float x, float y, float w, float h, float *volumeValue, const char *label)
{
    SDL_FPoint mousePos = {app->input.mouseX, app->input.mouseY};
    SDL_FRect trackRect = { x, y, w, h };

    // Made to allow easier scrolling of the volume bar
    SDL_FRect interactBounds = {
        x - 10.0f,
        y - 10.0f,
        w + 20.0f,
        h + 20.0f
    };

    bool isMouseOverSlider = SDL_PointInRectFloat(&mousePos, &interactBounds);
    float originalValue = *volumeValue;

    // Handle mouse wheel scrolling
    if (isMouseOverSlider && app->input.mouseWheelY != 0.0f)
    {
        *volumeValue += (app->input.mouseWheelY > 0.0f) ? 0.05f : -0.05f;
    }

    // Handle left click/drag
    if (app->input.mouseLeftDown && isMouseOverSlider)
    {
        float clickRelativeX = mousePos.x - x;
        float newVolume = clickRelativeX / w;
        *volumeValue = newVolume;
    }

    if (*volumeValue < 0.0f) *volumeValue = 0.0f;
    if (*volumeValue > 1.0f) *volumeValue = 1.0f;

    if (label)
    {
        SDL_FRect labelRect = { x, y - 35.0f, 150.0f, 30.0f };
        drawText(app, app->font, label, labelRect);
    }

    // =====================
    // RENDER VISUAL SLIDER
    // =====================
    SDL_SetRenderDrawBlendMode(app->renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(app->renderer, 40, 40, 40, 200);
    SDL_RenderFillRect(app->renderer, &trackRect);

    float fillW = w * (*volumeValue);
    if (fillW > 0.0f)
    {
        // Brighten up slider if mouse is over volume bar
        Uint8 trackBright = isMouseOverSlider ? 240 : 190;
        SDL_SetRenderDrawColor(app->renderer, trackBright, trackBright, trackBright, 255);
        SDL_FRect fillRect = { x, y, fillW, h };
        SDL_RenderFillRect(app->renderer, &fillRect);
    }

    SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, 255);
    SDL_RenderRect(app->renderer, &trackRect);

    if (fillW > 0.0f && fillW < w)
    {
        SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, 255);
        SDL_RenderLine(app->renderer, x + fillW, y, x + fillW, y + h);
    }

    // Percentage text
    char volStr[32];
    snprintf(volStr, sizeof(volStr), "%d%%", (int)SDL_lroundf((*volumeValue) * 100.0f));
    
    SDL_FRect displayRect = { x + w + 25.0f, y - 8.0f, 100.0f, 50.0f };
    drawText(app, app->font, volStr, displayRect);

    // Return true if this slider's value has changed during this frame execution
    return (*volumeValue != originalValue);
}

void volumeControls(AppState *app)
{
    float leftMargin = 100.0f;
    float startRowY = 400.0f;

    float sliderW = 400.0f;          
    float sliderH = 35.0f; 

    // Track if either slider value changed this frame
    bool changed = false;

    // 1. Process Master Slider (Row Y: 300)
    if (drawSlider(app, leftMargin, startRowY + 5.0f, sliderW, sliderH, &app->masterVolume, "Master Volume"))
    {
        changed = true;
    }

    // 2. Process Music Slider stacked cleanly below it (Row Y: 400)
    if (drawSlider(app, leftMargin, startRowY + 105.0f, sliderW, sliderH, &app->musicVolume, "Music Volume"))
    {
        changed = true;
    }

    // 3. Dynamically apply values using the mixed math scaling formula
    if (changed)
    {
        // Category Mix Rule: Music streams calculate (Master * Music)
        float combinedMusicVol = app->masterVolume * app->musicVolume;

        if (app->menuMusic.stream)     
            SDL_SetAudioStreamGain(app->menuMusic.stream, combinedMusicVol);
            
        if (app->techTreeMusic.stream) 
            SDL_SetAudioStreamGain(app->techTreeMusic.stream, combinedMusicVol);

        // Standard Category Rule: Ambient background audio uses Master directly 
        if (app->ambience.stream)      
            SDL_SetAudioStreamGain(app->ambience.stream, app->masterVolume);

        if (app->wrongSound.stream)      
            SDL_SetAudioStreamGain(app->wrongSound.stream, app->masterVolume);

        if (app->captureSound.stream)      
            SDL_SetAudioStreamGain(app->captureSound.stream, app->masterVolume);

        if (app->winSound.stream)      
            SDL_SetAudioStreamGain(app->winSound.stream, app->masterVolume);
        
        if (app->moveSound.stream)      
            SDL_SetAudioStreamGain(app->moveSound.stream, app->masterVolume);
    }
}

//player ui 
void techTreeButton(AppState *app){

    drawButton(app, &app->techTreeButton, "TECH TREE", 50.0f, WINDOW_HEIGHT - app->techTreeButton.h - 20.0f);
}

void playerRectangles(AppState *app) {
    float panelW = 400.0f;
    float panelH = 128.0f;
    float margin = 20.0f;

    // Player 1 (Blue - Left side)
    SDL_FRect p1GoldRect = { margin, margin, panelW, panelH };

    // Player 2 (Red - Right side)
    SDL_FRect p2GoldRect = { WINDOW_WIDTH - panelW - margin, margin, panelW, panelH };

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
    panel.x + 254.0f,
    panel.y + 70.0f,
    120.0f,
    30.0f
    };

    SDL_FRect townRect = {
        panel.x + 70.0f,
        panel.y + 70.0f,
        120.0f,
        30.0f
    };

    drawText(app, app->fontLarge, townStr, townRect);
    drawText(app, app->fontLarge, goldStr, goldRect);
}

SDL_Texture *getPieceTexture(AppState *app, PieceType type, UIState state)
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

void endMainMenuButton(AppState *app){
    drawButton(app, &app->mainmenubutton, "MAIN MENU", (float)(WINDOW_WIDTH / 2) - (app->mainmenubutton.w/2), 550.0f);
}

void endQuitButton(AppState *app){
    drawButton(app, &app->quitbutton, "QUIT", (float)(WINDOW_WIDTH / 2) - (app->quitbutton.w/2), 650.0f);
}


// Add this to menu.c
bool isMouseOverUI(AppState *app)
{
    // Checks if the mouse is in the bottom bar region (height 150px)
    if (app->input.mouseY >= (WINDOW_HEIGHT - 150.0f))
    {
        return true;
    }
    
    // You can add other global UI rectangle checks here if needed, like:
    // SDL_FPoint mousePt = { app->input.mouseX, app->input.mouseY };
    // if (SDL_PointInRectFloat(&mousePt, &app->techTreeButton)) return true;

    return false;
}

int getBasePieceCap(PieceType type)
{
    switch (type)
    {
        case PAWN:   case ENVOY:    return 8;
        case KNIGHT: case LANCER:   return 2;
        case BISHOP: case MAGE:     return 2;
        case ROOK:   case CATAPULT: return 2;
        case QUEEN:                 return 1;
        case KING:                  return 1;
        default:                    return 0;
    }
}

int getMaxPiecesForType(AppState *app, int playerNum, PieceType type)
{
    if (type == KING) return 1;

    int base = getBasePieceCap(type);
    TechTree *tree = (playerNum == 1) ? &app->techTreeP1 : &app->techTreeP2;
    
    if (isUpgradeUnlocked(tree, 10)) 
    {
        base *= 2;
    }

    int towns = (playerNum == 1) ? app->P1.towns : app->P2.towns;

    // Only apply extra pieces if they own MORE than their starting capital town
    if (towns > 1) {
        int bonusTowns = towns - 1; // Calculate extra expansions

        if (type == QUEEN) {
            base += (bonusTowns / 2); 
        }
        else if (type == PAWN || type == ENVOY) {
            base += (bonusTowns * 4); // +4 per expansion town
        }
        else {
            base += (bonusTowns * 1); // +1 per expansion town for other pieces
        }
    }

    return base;
}

int countPiecesByType(AppState *app, int player, PieceType type) {
    int total = 0;
    // Walk the entire structural array buffer using your native variable
    for (int i = 0; i < app->maxPieceCapacity; i++) {
        if (app->pieces[i].active && app->pieces[i].owner == player && app->pieces[i].type == type) {
            total++;
        }
    }
    return total;
}

bool isPieceAtCap(AppState *app, int player, PieceType type)
{
    int count = 0;

    // Count base + upgraded pieces (same logic you already had)
    for (int i = 0; i < app->maxPieceCapacity; i++)
    {
        if (app->pieces[i].active &&
            app->pieces[i].owner == player &&
            app->pieces[i].type == type)
        {
            count++;
        }
    }

    // Include upgrade equivalents so base slots behave correctly
    switch (type)
    {
        case PAWN:   count += countPiecesByType(app, player, ENVOY);    break;
        case KNIGHT: count += countPiecesByType(app, player, LANCER);   break;
        case BISHOP: count += countPiecesByType(app, player, MAGE);     break;
        case ROOK:   count += countPiecesByType(app, player, CATAPULT); break;
        default: break;
    }

    int maxAllowed = getMaxPiecesForType(app, player, type);

    return count >= maxAllowed;
}