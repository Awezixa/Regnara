#include "cheats.h"

void infiniteMoney(AppState *app){
    int current = (app->currentPlayer == 1) ? 1 : 2;

    if (current == 1)
    {
        app->P1.p1Gold = 100;
    }
    else if(current == 2){
        app->P2.p2Gold = 100;
    }
}

void drawCheats(AppState *app){
    app->cheatText.x = (float)(WINDOW_WIDTH / 2) - 50.0f;
    app->cheatText.y = 60.0f;
    app->cheatText.w = 70.0f;
    app->cheatText.h = 60.0f;

    drawText(app, app->font, "CHEATS ON" , app->cheatText);
}

void presetMatch(AppState *app) {
    app->cheats = false;
    
    app->pieceCount = 0;
    app->P1.pieceCount = 0;
    app->P2.pieceCount = 0;
    app->P1.p1Gold = 26;
    app->P2.p2Gold = 57;
    app->turnCounter = 23;

    for(int i = 0; i < app->maxPieceCapacity; i++) app->pieces[i].active = false;

    for (int i = 0; i <= 11; i++) {
        app->techTreeP1.upgrades[i].unlocked = 1;
        app->techTreeP2.upgrades[i].unlocked = 1;
    }

    app->P1.towns = 0;
    app->P2.towns = 0;
    for (int t = 0; t < app->tTowns; t++) {
        if (t < 3) {
            app->towns[t].owner = 1;
            app->P1.towns++;
        } else if (t < 6) { 
            app->towns[t].owner = 2;
            app->P2.towns++;
        }
    }
    struct {int r; int c; pieceType type; int owner;} presets[] = {
        {14, 9, KING, 1}, {14, 6, KNIGHT, 1}, {10, 39, BISHOP, 1}, {24, 10, ROOK, 1}, {9, 37, KNIGHT, 1}, {8, 42, ENVOY, 1},{23, 12, CATAPULT, 1},{12, 12, ENVOY, 1},// P1 Pieces
        {32, 8, KING, 2}, {30, 9, KNIGHT, 2}, {29, 43, BISHOP, 2}, {26, 14, ROOK, 2}, {32, 45, ENVOY, 2}, {12, 35, QUEEN, 2}, {23, 15, LANCER, 2}, {25, 17, ENVOY, 2}, // P2 Pieces
    };

    int numPresets = sizeof(presets) / sizeof(presets[0]); 
    for (int i = 0; i < numPresets; i++) { 
        app->pieces[i].active = true;
        app->pieces[i].row = presets[i].r;
        app->pieces[i].col = presets[i].c;
        app->pieces[i].pieceX = (float)(presets[i].c * TILE_SIZE);
        app->pieces[i].pieceY = (float)(presets[i].r * TILE_SIZE);
        app->pieces[i].type = presets[i].type;
        app->pieces[i].owner = presets[i].owner;
        app->pieces[i].moved = false;

        app->pieceCount++;
        if (presets[i].owner == 1) app->P1.pieceCount++;
        else app->P2.pieceCount++;
    }
}











