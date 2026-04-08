#include "moves.h"
#include "piece.h"
#include "../game.h"

void GenerateKingMoves(AppState *app, Piece *p)
{
    app->possibleMoveCount = 0;

    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {

            if (dx == 0 && dy == 0) continue;

            int newCol = p->pieceX + dx;
            int newRow = p->pieceY + dy;

            if (newCol < 0 || newCol >= 8 || newRow < 0 || newRow >= 8)
                continue;

            app->possibleMoves[app->possibleMoveCount++] =
                (SDL_Point){newCol, newRow};
        }
    }
}

void GenerateMoves(AppState *app, Piece *p)
{
    switch (p->type)
    {
        case PIECE_KING:
            GenerateKingMoves(app, p);
            break;
        case PIECE_KNIGHT:
            break;
        case PIECE_PAWN:
            break;
    }
}