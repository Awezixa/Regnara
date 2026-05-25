#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL_main.h>
#include <stdlib.h>
#include <time.h>

//file includes
#include "game.h"
#include "Pieces/moves.h"
#include "techTree/techTree.h"
static void UpdateCamera(AppState *app);
//clamp function = restriction of value to specific range


SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    (void)argc;
    (void)argv; // unused

    AppState *app = (AppState *)SDL_calloc(1, sizeof(AppState));
    if (!app)
    {
        SDL_Log("Out of memory.");
        return SDL_APP_FAILURE;
    }
    *appstate = app;

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
    {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!TTF_Init())
    {
        SDL_Log("TTF_Init failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // create window and renderer
    if (!SDL_CreateWindowAndRenderer(APP_NAME, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_FULLSCREEN, &app->window, &app->renderer))
    {
        SDL_Log("SDL_CreateWindowAndRenderer failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_SetRenderLogicalPresentation(app->renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    //font loading
    app->font = TTF_OpenFont("Assets/FORCED SQUARE.ttf", 30);
    app->fontLarge = TTF_OpenFont("Assets/FORCED SQUARE.ttf", 50);
    if (!app->font)
    {
        SDL_Log("TTF_OpenFont failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    //Audio initialization(s)
    if (sdl_initialize_audio() == SDL_APP_FAILURE)
    {
        SDL_Log("Audio system failed to initialize. Continuing without sound.");
    }
    else
    {
        init_sound("Assets/audio/regnaraMenuMusic.wav", &app->menuMusic);
        init_sound("Assets/audio/techTreeMusic.wav", &app->techTreeMusic);
       
    }

    if (!LoadAllGameTextures(app)) {
        return SDL_APP_FAILURE; 
    }


    //other game initializations
    app->gameState = STATE_MENU;
    loadMap("Assets/maps/map1.txt");
    app->piecePlaced = false;
    //world size initializations
    app->worldSize.x = (float)(MAP_COLS * TILE_SIZE);
    app->worldSize.y = (float)(MAP_ROWS * TILE_SIZE);

    float centerX = (app->worldSize.x - (WINDOW_WIDTH/1.0f)) / 2.0f;
    float centerY = (app->worldSize.y - (WINDOW_HEIGHT/1.0f)) / 2.0f;
    //camera initial ization 
    camera2d_init(&app->camera, centerX, centerY, 1.0f);

    //piece array initialization
    
    app->cheats = false;
    app->inGame = false;
    app->maxPieceCapacity = 20;
    app->pieces = (Piece *) malloc(sizeof(Piece) * app->maxPieceCapacity);
    app->pieceCount = 0;
    app->maxPlayerPieces = 9;
    for (int i = 0; i < app->maxPieceCapacity; i++){
        app->pieces[i].active = false;
    }

    //player initialization
    app->currentPlayer = 1;
    app->turnCounter = 1;
    app->winner = 0;
    app->P1.p1Gold = 10;
    app->P2.p2Gold = 10;
    app->errorTimer = 0.0f;
    app->inGame = false;
    app->showTechTree = false;
    
    app->masterVolume = 0.7f;

    // TECH TREE INIT
    initTechTree(&app->techTreeP1);
    initTechTree(&app->techTreeP2);
    //town initialization
    initTowns(app);

    app->lastTicksMS = SDL_GetTicks();
    app->dt = 0.0f;

    // HARD RESET: Lock the tech tree music tracking values on startup
    stopSound(&app->techTreeMusic);

    // PLAY: Wake up the main menu theme cleanly
    if (app->menuMusic.stream)
    {
        playSound(&app->menuMusic);
    }
    

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    AppState *app = (AppState *)appstate;
    InputState *in = &app->input;
    GameState *gamestate = &app->gameState;

    switch (event->type)
    {
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;
    case SDL_EVENT_KEY_DOWN:
    {
        SDL_Scancode sc = event->key.scancode;
        bool repeat = event->key.repeat != 0;
        if (sc >= 0 && sc < SDL_SCANCODE_COUNT)
        {
            if (!repeat && !in->keyDown[sc])
                in->keyPressed[sc] = true;
            in->keyDown[sc] = true;
        }
    }
    break;
    case SDL_EVENT_KEY_UP:
    {
        SDL_Scancode sc = event->key.scancode;
        if (sc >= 0 && sc < SDL_SCANCODE_COUNT)
        {
            in->keyDown[sc] = false;
            in->keyReleased[sc] = true;
        }
    }
    break;
    case SDL_EVENT_MOUSE_MOTION:{
        float logicalX, logicalY;
        SDL_RenderCoordinatesFromWindow(app->renderer, 
                                           event->motion.x, 
                                           event->motion.y, 
                                           &logicalX, &logicalY);
        in->mouseX = logicalX;
        in->mouseY = logicalY;
    }
    break;
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
        {float logicalX, logicalY;
        SDL_RenderCoordinatesFromWindow(app->renderer, 
                                           event->button.x, 
                                           event->button.y, 
                                           &logicalX, &logicalY);
        in->mouseX = logicalX;
        in->mouseY = logicalY;}

        if (event->button.button == SDL_BUTTON_LEFT)
        {
            if (!in->mouseLeftDown)
                in->mouseLeftPressed = true;
            in->mouseLeftDown = true;
        }
        //right mouse button checks
        else if (event->button.button == SDL_BUTTON_RIGHT)
        {
            if (!in->mouseRightDown)
                in->mouseRightPressed = true;
            in->mouseRightDown = true;
        }
        else if (event->button.button == SDL_BUTTON_MIDDLE) {
            if (!in->mouseMiddleDown) in->mouseMiddlePressed = true;
            in->mouseMiddleDown = true;
        }
        break;
    case SDL_EVENT_MOUSE_BUTTON_UP:
        if (event->button.button == SDL_BUTTON_LEFT)
        {
            in->mouseLeftDown = false;
            in->mouseLeftReleased = true;
        }
        else if (event->button.button == SDL_BUTTON_RIGHT)
        {
            in->mouseRightDown = false;
            in->mouseRightReleased = true;
        }
        else if (event->button.button == SDL_BUTTON_MIDDLE){
            in->mouseMiddleDown = false;
            in->mouseMiddleReleased = true;
        } 
        break;
    case SDL_EVENT_MOUSE_WHEEL:
        // event.wheel.y is positive for up (zoom in) and negative for down (zoom out)
        in->mouseWheelY = event->wheel.y;
        break;
    default:
        break;
    }

    return SDL_APP_CONTINUE;
}

//input resets for mouse and keys
void Input_BeginFrame(InputState *input)
{
    input->mouseLeftPressed = false;
    input->mouseLeftReleased = false;
    input->mouseRightPressed = false;
    input->mouseRightReleased = false;
    input->mouseMiddlePressed = false;
    input->mouseMiddleReleased = false;
    input->mouseWheelY = 0.0f;

    for (int i = 0; i < SDL_SCANCODE_COUNT; ++i)
    {
        input->keyPressed[i] = false;
        input->keyReleased[i] = false;
    }
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    AppState *app = (AppState *)appstate;

    // =========================
    // Delta time
    // =========================
    Uint64 nowMS = SDL_GetTicks();
    Uint64 elapsedMS = nowMS - app->lastTicksMS;
    app->lastTicksMS = nowMS;
    app->dt = (float)elapsedMS / 1000.0f;

if (app->gameState == STATE_MENU && app->menuMusic.stream)
    {
        app->menuMusic.playbackTimer -= app->dt;
        if (app->menuMusic.playbackTimer <= 0.0f) {
            playSound(&app->menuMusic); // Rewinds and loops theme naturally
        }
    }
    else if (app->gameState == TECH_TREE && app->techTreeMusic.stream)
    {
        app->techTreeMusic.playbackTimer -= app->dt;
        if (app->techTreeMusic.playbackTimer <= 0.0f) {
            playSound(&app->techTreeMusic); // Rewinds and loops tech theme naturally
        }
    }

    SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
    SDL_RenderClear(app->renderer);

    // =========================
    // GAME STATES
    // =========================
    switch (app->gameState)
    {
    case STATE_MENU:
    {
        drawMainMenu(app);

        if (app->input.mouseLeftPressed)
        {
            SDL_FPoint mousePos = {app->input.mouseX, app->input.mouseY};
            
            if (SDL_PointInRectFloat(&mousePos, &app->playbutton)){
                stopSound(&app->menuMusic); // Resets timer to 0
                resetGame(app);
                app->gameState = STATE_PLAYING;
            }          

            if (SDL_PointInRectFloat(&mousePos, &app->quitbutton)){
                return SDL_APP_SUCCESS;
            }

            if (SDL_PointInRectFloat(&mousePos, &app->optionsbutton)){
                app->gameState = STATE_OPTIONS;
            }
        }

        if (app->input.keyPressed[SDL_SCANCODE_SPACE]||app->input.keyPressed[SDL_SCANCODE_KP_ENTER])
            app->gameState = STATE_PLAYING;

        break;
    }
    //done to have overlay instead of specific state
case STATE_PLAYING:
    {
        app->inGame = true;
        updateGame(app); // Run core gameplay normally
        break;
    }

    case TECH_TREE:
    {

        app->inGame = true;
        
        // 2. STATIC BACKGROUND LAYER RENDERING
        // We call these baseline functions manually to freeze inputs while keeping the map visible underneath!
        UpdateCamera(app);
        renderMap(app->renderer, app);
        drawTerritory(app);
        renderPiece(app);
        if (app->selectedPiece != NULL) {
            PossibleMovesShow(app, app->selectedPiece);
        }
        turnUI(app);
        playerRectangles(app);
        gameUI(app);

        // 3. DRAW TECH TREE WINDOW BARRIER OVERLAY ON TOP
        SDL_SetRenderDrawBlendMode(app->renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 150);
        SDL_FRect screen = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
        SDL_RenderFillRect(app->renderer, &screen);

        techTreeOverlay(app);

        // Handle purchasing gold feedback error alerts safely
        if (app->errorTimer > 0.0f) {
            app->errorTimer -= app->dt;
            SDL_FRect errPos = { 400, 800, 400, 255 };
            drawText(app, app->font, "Not enough gold!", errPos);
        }

        // 4. CLEAN EXITS: Gracefully stop the track and shift states back to active gameplay
        if (app->input.keyPressed[SDL_SCANCODE_ESCAPE] || app->input.keyPressed[SDL_SCANCODE_T]) 
        {
            stopSound(&app->techTreeMusic); // Turn off the track, sets counter to 0
            app->gameState = STATE_PLAYING; // Return to standard map view
        }

        break;   
    }

    case STATE_OPTIONS:
    {

        drawOptions(app);
        if (app->input.keyPressed[SDL_SCANCODE_ESCAPE] && app->inGame == true ){
            app->gameState = STATE_PAUSED;
        }
        else if(app->input.keyPressed[SDL_SCANCODE_ESCAPE] && app->inGame == false){
            app->gameState = STATE_MENU;
        }
        break;
    }

    case STATE_PAUSED:
    {
        drawPauseMenu(app);

        if (app->input.keyPressed[SDL_SCANCODE_ESCAPE]){
            app->gameState = STATE_PLAYING;
        }
        if (app->input.mouseLeftPressed)
        {
            SDL_FPoint mousePos = {app->input.mouseX, app->input.mouseY};
            if (SDL_PointInRectFloat(&mousePos, &app->quitbutton)){
            //add confirmation quit screen
            return SDL_APP_SUCCESS;
            }
            if (SDL_PointInRectFloat(&mousePos, &app->optionsbutton)){
            app->gameState = STATE_OPTIONS;
            if (app->input.keyPressed[SDL_SCANCODE_ESCAPE]){
                    app->gameState = STATE_PAUSED;
                }
            }
            if (SDL_PointInRectFloat(&mousePos, &app->mainmenubutton)){
                //add confirmation quit screen
                stopSound(&app->techTreeMusic);
                
                // Restart and re-loop the title screen menu music
                playSound(&app->menuMusic);
                
                app->gameState = STATE_MENU;
            }

        }    

        break;
    }

    case STATE_END:
    {
        drawEndScreen(app);
        SDL_FPoint mousePos = {app->input.mouseX, app->input.mouseY};
        if (app->input.mouseLeftPressed){
            if (app->input.keyPressed[SDL_SCANCODE_RETURN]||(SDL_PointInRectFloat(&mousePos, &app->mainmenubutton))){
                    app->gameState = STATE_MENU;
                }
            if(SDL_PointInRectFloat(&mousePos, &app->quitbutton)){
                return SDL_APP_SUCCESS;
            }
        }
        
        break;
    }

    default:
        break;
    }

    SDL_RenderPresent(app->renderer);

    // reset input
    Input_BeginFrame(&app->input);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    //clean up
    (void)result;
    AppState *app = (AppState *)appstate;
    if (!app) return;


    if (app->menuMusic.stream) {
        stopSound(&app->menuMusic);
        SDL_DestroyAudioStream(app->menuMusic.stream);
        SDL_free(app->menuMusic.wav_data);
    }

    if(app->pieces)free(app->pieces);
    //add function to destroy all textures
    CleanupAllTextures(app);

    if (app->font)TTF_CloseFont(app->font);
    if (app->renderer)SDL_DestroyRenderer(app->renderer);
    if (app->window)SDL_DestroyWindow(app->window);

    TTF_Quit();
    SDL_Quit();
    SDL_free(app);
}

















// =========================
// OTHER FUNCTIONS
// =========================



void UpdateCamera(AppState *app){
    InputState *in = &app->input;
    Camera *camera = &app->camera;

    //for camera zooming and following
    //updating every second on screen
    float zoomDelta = in->mouseWheelY * 15.0f;
    float zoomSpeed = 10.0f;

    if (in->keyDown[SDL_SCANCODE_Q]) zoomDelta += zoomSpeed;
    if (in->keyDown[SDL_SCANCODE_E]) zoomDelta -= zoomSpeed;
    // Instead of camera2d_add_zoom(&app->camera, zoomDelta * app->dt, 0.4f, 5.0f);
    camera2d_zoom_to_mouse(&app->camera, zoomDelta * app->dt, 0.4f, 5.0f, app->input.mouseX, app->input.mouseY);//adjusts camera clamp range then follows player with new clamped range

    //drag logic
    if (in->mouseMiddlePressed)
    {
        app->pivotX = in->mouseX;
        app->pivotY = in->mouseY;
        app->camX = camera->x;
        app->camY = camera->y;
    }
    if (in->mouseMiddleDown)
    {
        float mouseDeltaX = in->mouseX - app->pivotX;
        float mouseDeltaY = in->mouseY - app->pivotY;

        // Convert screen-space drag into world-space camera movement.
        camera->x = app->camX - (mouseDeltaX / camera->zoom);
        camera->y = app->camY - (mouseDeltaY / camera->zoom);
    }

    //Cesar's tip to clamp camera in "game view"
    float viewW = (float)WINDOW_WIDTH / camera->zoom;
    float viewH = (float)WINDOW_HEIGHT / camera->zoom;
    

    //math and check to ensure that camera.x and camera.y is never < 0
    //max can go is total world size - visible view port
    float maxX = app->worldSize.x - viewW;
    float maxY = app->worldSize.y - viewH;
    float min_x = 0.0f;
    float min_y = 0.0f;

    if(maxX < 0){
        min_x = maxX / 2.0f;
        maxX = min_x;
    } 
    if(maxY < 0){
        min_y = maxY / 2.0f;
        maxY = min_y;
    }

    // camera->x = SDL_clamp(camera->x, min_x, maxX);
    // camera->y = SDL_clamp(camera->x, min_y, maxY);
}

void updateGame(AppState *app) {
    
    // 1. UPDATE CORE SYSTEMS & RENDERING WORLD BACKGROUND FIRST
    UpdateCamera(app);
    renderMap(app->renderer, app);

    // 2. DRAW GAME BOARD LAYERS IN BACKGROUND
    drawTerritory(app); // Render the 3x3 town colors
    renderPiece(app);    // Render active units on top of territory

    if (app->selectedPiece != NULL) {
        PossibleMovesShow(app, app->selectedPiece); // Show valid movement spots
    }

    // =========================================================
    // 3. CONDITIONAL INTERACTION BLOCK (Gated by Tech Tree)
    // =========================================================
    SDL_FPoint mousePos = {app->input.mouseX, app->input.mouseY};

    if (!app->showTechTree) 
    {
        // Only run unit spawning and board mechanics if the overlay is hidden!
        
        // Piece Selection Shortcuts/Hotkeys
        if (app->input.mouseLeftPressed) {
            if (SDL_PointInRectFloat(&mousePos, &app->pawnButton)) {app->selectedPieceType = PAWN;}
            else if (SDL_PointInRectFloat(&mousePos, &app->knightButton)) {app->selectedPieceType = KNIGHT;} 
            else if (SDL_PointInRectFloat(&mousePos, &app->bishopButton)) {app->selectedPieceType = BISHOP;} 
            else if (SDL_PointInRectFloat(&mousePos, &app->rookButton)) {app->selectedPieceType = ROOK;} 
            else if (SDL_PointInRectFloat(&mousePos, &app->queenButton)) {app->selectedPieceType = QUEEN;}
            else if (SDL_PointInRectFloat(&mousePos, &app->envoyButton)) {app->selectedPieceType = ENVOY;}
            else if (SDL_PointInRectFloat(&mousePos, &app->lancerButton)) {app->selectedPieceType = LANCER;}
            else if (SDL_PointInRectFloat(&mousePos, &app->mageButton)) {app->selectedPieceType = MAGE;}
            else if (SDL_PointInRectFloat(&mousePos, &app->catapultButton)) {app->selectedPieceType = CATAPULT;}
        }
        if (app->input.keyDown[SDL_SCANCODE_1]) app->selectedPieceType = PAWN;
        if (app->input.keyDown[SDL_SCANCODE_2]) app->selectedPieceType = KNIGHT;
        if (app->input.keyDown[SDL_SCANCODE_3]) app->selectedPieceType = BISHOP;
        if (app->input.keyDown[SDL_SCANCODE_4]) app->selectedPieceType = ROOK;
        if (app->input.keyDown[SDL_SCANCODE_5]) app->selectedPieceType = QUEEN;
        if (app->input.keyDown[SDL_SCANCODE_6]) app->selectedPieceType = ENVOY;
        if (app->input.keyDown[SDL_SCANCODE_7]) app->selectedPieceType = LANCER;
        if (app->input.keyDown[SDL_SCANCODE_8]) app->selectedPieceType = MAGE;
        if (app->input.keyDown[SDL_SCANCODE_9]) app->selectedPieceType = CATAPULT;

        // FIXED: Spawning and board checking runs uniformly outside of old broken triggers
        spawnPiece(app);

        // --- Core Piece Selection & Movement State Machine ---
        int mouseCol = (int)((app->input.mouseX / app->camera.zoom + app->camera.x) / TILE_SIZE);
        int mouseRow = (int)((app->input.mouseY / app->camera.zoom + app->camera.y) / TILE_SIZE);
        bool wasPieceSelected = (app->selectedPiece != NULL);

        for (int i = 0; i < app->maxPieceCapacity; i++) {
            Piece *p = &app->pieces[i];
            if (p->active && p->col == mouseCol && p->row == mouseRow) {
                if (app->input.mouseLeftPressed && app->selectedPiece == NULL && p->owner == app->currentPlayer && !p->moved) {
                    app->selectedPiece = p;
                    GenerateMoves(app, p);
                }
            }
        }

        if (app->cheats && app->selectedPiece && app->input.mouseLeftPressed && wasPieceSelected) {
            app->selectedPiece->col = mouseCol;
            app->selectedPiece->row = mouseRow;
            app->selectedPiece->pieceX = mouseCol * TILE_SIZE;
            app->selectedPiece->pieceY = mouseRow * TILE_SIZE;
            app->selectedPiece = NULL;
        }
        else if (app->selectedPiece && app->input.mouseLeftPressed && wasPieceSelected) {
            bool actionTaken = false;
            for (int i = 0; i < app->possibleMoveCount; i++) {
                SDL_Point m = app->possibleMoves[i];
                if (m.x != mouseCol || m.y != mouseRow) continue;
                if (!isTileWalkable(mouseRow, mouseCol)) continue;

                Piece *target = GetPieceAt(app, mouseRow, mouseCol);
                if (target && target->owner == app->selectedPiece->owner) continue;

                if (target && target->owner != app->selectedPiece->owner) {
                    if (app->selectedPiece->type == LANCER) LancerAttack(app, app->selectedPiece, mouseRow, mouseCol);
                    else CapturePiece(app, mouseRow, mouseCol);
                }

                app->selectedPiece->col = mouseCol;
                app->selectedPiece->row = mouseRow;
                app->selectedPiece->pieceX = mouseCol * TILE_SIZE;
                app->selectedPiece->pieceY = mouseRow * TILE_SIZE;
                app->selectedPiece->moved = true;
                actionTaken = true;
                break;
            }

            if (!actionTaken) {
                for (int i = 0; i < app->possibleAttackCount; i++) {
                    SDL_Point a = app->possibleAttacks[i];
                    if (a.x != mouseCol || a.y != mouseRow) continue;

                    Piece *target = GetPieceAt(app, mouseRow, mouseCol);
                    if (!target || target->owner == app->selectedPiece->owner) continue;

                    if (app->selectedPiece->type == MAGE) CapturePiece(app, mouseRow, mouseCol);
                    else if (app->selectedPiece->type == CATAPULT) CatapultAttack(app, mouseRow, mouseCol);

                    app->selectedPiece->moved = true;
                    actionTaken = true;
                    break;
                }
            }

            if (actionTaken || (mouseCol == app->selectedPiece->col && mouseRow == app->selectedPiece->row)) {
                app->selectedPiece = NULL;
                app->possibleMoveCount = 0;
                app->possibleAttackCount = 0;
            }
        }

        // Handle Cheats Overlay Toggle
        if (app->input.keyPressed[SDL_SCANCODE_C]) app->cheats = !app->cheats;
        if (app->cheats) {
            drawCheats(app);
            infiniteMoney(app);
            if (app->input.keyPressed[SDL_SCANCODE_J]) {
                presetMatch(app);
                app->maxPlayerPieces = 15;
            }
        }

        // Standard Pause Check
        if (app->input.keyPressed[SDL_SCANCODE_ESCAPE] && app->gameState == STATE_PLAYING) {
            app->gameState = STATE_PAUSED;
        }
    }

    // =========================================================
    // 4. DRAW SYSTEM UI & HEADS-UP INTERFACES ON TOP OF WORLD
    // =========================================================
    turnUI(app);
    playerRectangles(app);
    gameUI(app); // Draws standard action bar HUD panel

    if (app->input.mouseLeftPressed) {
        if (SDL_PointInRectFloat(&mousePos, &app->endTurnButton)) {
            endTurn(app);
        }
    }

    // =========================================================
    // 5. TECH TREE TRANSITION TRIGGER (State Machine Switch Only)
    // =========================================================
    if (app->input.keyPressed[SDL_SCANCODE_T] || 
       (app->input.mouseLeftPressed && SDL_PointInRectFloat(&mousePos, &app->techTreeButton))) 
    {
        // Hard-stop anything playing in the background match view
        stopSound(&app->menuMusic);
        
        // Start playing the tech theme exactly once on this trigger frame
        playSound(&app->techTreeMusic);
        
        // Switch execution state layers safely
        app->gameState = TECH_TREE; 
    }

    winCondition(app); // Process target objective evaluations
}

void endTurn(AppState *app) {

    //reset all pieces for movement
    for (int i = 0; i < app->maxPieceCapacity; i++)
    {
        app->pieces[i].moved = false;
    }

    // 1. Process capture for the player who just finished their turn
    townCaptured(app);

    // 2. Switch the active player
    if (app->currentPlayer == 1) {
        app->currentPlayer = 2;
    } else {
        app->currentPlayer = 1;
        app->turnCounter++;
    }

    app->selectedPiece = NULL;
    app->selectedPieceType = PAWN;

    snapToKing(app);

    // 3. Give gold to the NEW active player (start of turn income)
    if (app->currentPlayer == 1) {

        int bonus = isUpgradeUnlocked(&app->techTreeP1, 2) ? 5 : 0;
        app->P1.p1Gold += 7 + bonus;

    } else if(app->currentPlayer == 2){
        
        int bonus = isUpgradeUnlocked(&app->techTreeP2, 2) ? 5 : 0;
        app->P2.p2Gold += 7 + bonus;
    }
    
    // 4. Cap the gold
    if (app->P1.p1Gold > 100) app->P1.p1Gold = 100;
    if (app->P2.p2Gold > 100) app->P2.p2Gold = 100;
}

void winCondition(AppState *app){
    //need to check who controls all towns

    if (app->P1.towns == app->tTowns && app->tTowns > 0)
    {
        app->winner = 1;
        app->gameState = STATE_END;
        
    }
    else if(app->P2.towns == app->tTowns && app->tTowns > 0){
        app->winner = 2;
        app->gameState = STATE_END;
    }
    //king capturing win condition in piece capturing
}

void resetGame(AppState *app) {
   
    for (int i = 0; i < app->tTowns; i++) {
        app->towns[i].owner = 0;
        app->towns[i].captureTurns = 0;
    }

    for (int i = 0; i < app->maxPieceCapacity; i++) {
        app->pieces[i].active = false;
        app->pieces[i].owner = 0;
    }
    
    app->pieceCount = 0;
    app->selectedPiece = NULL;
    app->possibleMoveCount = 0;
    
    
    app->currentPlayer = 1;
    app->turnCounter = 1;
    app->winner = 0;

    app->P1.p1Gold = 10;
    app->P1.towns = 0;
    app->P1.pieceCount = 0;

    app->P2.p2Gold = 10;
    app->P2.towns = 0;

    app->P2.pieceCount = 0;

    app->cheats = false;
    app->showTechTree = false;
  
    initTechTree(&app->techTreeP1);
    initTechTree(&app->techTreeP2);
    initTowns(app);
    startGame(app);

    snapToKing(app);
}

void startGame(AppState *app){

    srand(time(NULL));

    bool p1Spawned = false;
    bool p2Spawned = false;
    int slot = 0;

    for (int r = 0; r < MAP_ROWS; r++)
    {
        for (int c = 0; c < MAP_COLS; c++)
        {
            
            if (map_data[r][c]== SPAWN_POINT)
            {
                if (slot < app->maxPieceCapacity)
                {
                    app->pieces[slot].active = true;
                    app->pieces[slot].type = KING;
                    app->pieces[slot].row = r;
                    app->pieces[slot].col = c;
                    app->pieces[slot].pieceX = (float)(c * TILE_SIZE);
                    app->pieces[slot].pieceY = (float)(r * TILE_SIZE);
                }

                //spawn p1 first then p2
                if (p1Spawned == false)
                {
                    app->pieces[slot].owner = 1;
                    p1Spawned = true;
                    app->P1.pieceCount++;
                }else if(p2Spawned == false){
                    app->pieces[slot].owner = 2;
                    p2Spawned = true;
                    app->P2.pieceCount++;
                }
                slot++;
            }
        }
        
    }
    
}

void increaseTroopLimit(AppState *app, int extraSlots){
    int oldCapacity = app->maxPieceCapacity;
    app->maxPieceCapacity += extraSlots;

    Piece *newSpace = (Piece *)realloc(app->pieces, sizeof(Piece)* app->maxPieceCapacity);
    //can modify when tech tree working
    if(newSpace){
        app->pieces = newSpace;

        for(int i = oldCapacity; i < app->maxPieceCapacity; i++){
            app->pieces[i].active = false;
        }

    }
}

void snapToKing(AppState *app){
    float kingX = 0.0f;
    float kingY = 0.0f;
    bool found = false;

    for (int i = 0; i < app->maxPieceCapacity; i++) {
        if (app->pieces[i].active && 
            app->pieces[i].type == KING && 
            app->pieces[i].owner == app->currentPlayer) 
        {
            // Center calculation: get the middle point of the tile
            kingX = app->pieces[i].pieceX + (TILE_SIZE / 2.0f);
            kingY = app->pieces[i].pieceY + (TILE_SIZE / 2.0f);
            found = true;
            break;
        }
    }

    if (found) {
        camera2d_follow_target(
            &app->camera,
            kingX,
            kingY,
            (float)WINDOW_WIDTH,
            (float)WINDOW_HEIGHT,
            app->worldSize.x,
            app->worldSize.y
        );
    }

}