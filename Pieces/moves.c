#include "../game.h"
#include "piece.h"
#include "moves.h"

void GenerateMoves(AppState *app, Piece *p)
{
    switch (p->type)
    {
        case KING:
            GenerateKingMoves(app, p);
            break;
        case KNIGHT:
            // later
            break;
        case PAWN:
            GenerateKingMoves(app, p);
            break;
    }
}

void GenerateKingMoves(AppState *app, Piece *p)
{
    app->possibleMoveCount = 0;

    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {

            if (dx == 0 && dy == 0) continue;

            int newCol = p->col + dx;
            int newRow = p->row + dy;

            // Stay inside board
            if (newCol < 0 || newCol >= MAP_COLS || newRow < 0 || newRow >= MAP_ROWS)
                continue;

            app->possibleMoves[app->possibleMoveCount++] =
                (SDL_Point){newCol, newRow};
        }
    }
}

void PossibleMovesShow(AppState *app, Piece *p)
{
    for (int i = 0; i < app->possibleMoveCount; i++)
    {
        SDL_FRect r = {
        app->possibleMoves[i].x * TILE_SIZE - app->camX,
        app->possibleMoves[i].y * TILE_SIZE - app->camY,
        TILE_SIZE,
        TILE_SIZE
    };

        SDL_RenderTexture(app->renderer, app->movePossible, NULL, &r);
    }
}