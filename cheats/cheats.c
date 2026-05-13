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

void presetMatch(AppState *app){
    
}












