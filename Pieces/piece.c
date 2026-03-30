#include "piece.h"
#include "../game.h"
void spawnPiece(AppState *app){

    if (app->input.mouseLeftPressed) {
        app->piecePlaced = true;
        app->pieceX = app->input.mouseX;
        app->pieceY = app->input.mouseY;
    }
}

void renderPiece(AppState *app){

    SDL_FRect p = {app->pieceX, app->pieceY, 32.0f, 32.0f};
    SDL_RenderTexture(app->renderer, app->pawnTexture, NULL, &p);
}

