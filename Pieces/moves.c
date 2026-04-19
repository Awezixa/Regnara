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
            GenerateKnightMoves(app,p);
            break;
        case BISHOP:
            GenerateBishopMoves(app,p);
            break;
        case ROOK:
            GenerateRookMoves(app,p);
            break;
        case QUEEN:
            GenerateQueenMoves(app,p);
            break;
        case PAWN:
            GenerateKingMoves(app, p);
            break;
    }
}

bool isTileOccupied(AppState *app, int row, int col)
{
    for (int i = 0; i < MAX_PIECES; i++)
    {
        Piece *other = &app->pieces[i];

        if (other->active &&
            other->row == row &&
            other->col == col &&
            ((app->currentPlayer == 1 && app->pieces[i].owner == 1) || (app->currentPlayer == 2 && app->pieces[i].owner == 2)))
        {
            return true; // something is there
        }
    }
    return false; // empty
}

void CapturePiece(AppState *app, int row, int col)
{
    for (int i = 0; i < MAX_PIECES; i++)
    {
        Piece *other = &app->pieces[i];

        if (other->active &&
            other->row == row &&
            other->col == col)
        {
            other->active = false; // 💀 remove piece
            return;
        }
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
            if (newCol < 0 || newCol >= MAP_COLS ||
                newRow < 0 || newRow >= MAP_ROWS)
                continue;

            // 🚫 Block if tile is occupied (no capturing)
            if (isTileOccupied(app, newRow, newCol))
                continue;

            CapturePiece(app,newRow,newCol);

            app->possibleMoves[app->possibleMoveCount++] =
                (SDL_Point){newCol, newRow};
        }
    }
}

void GenerateKnightMoves(AppState *app, Piece *p)
{
    app->possibleMoveCount = 0;

    for (int dx = -2; dx <= 2; dx++) {
        for (int dy = -2; dy <= 2; dy++) {
            
            
            if ((dx == 0 && dy == 0) || (abs(dx) == 2 && abs(dy) == 2)) continue;

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

void GenerateBishopMoves(AppState *app, Piece *p)
{
    app->possibleMoveCount = 0;

    for (int dx = -3; dx <= 3; dx++) {
        for (int dy = -3; dy <= 3; dy++) {
            
            
            if ((dx == 0 && dy == 0) || !(abs(dx) == abs(dy) || (abs(dx) + abs(dy) == 1))) continue;

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

void GenerateRookMoves(AppState *app, Piece *p)
{
    app->possibleMoveCount = 0;

    for (int dx = -3; dx <= 3; dx++) {
        for (int dy = -3; dy <= 3; dy++) {
            
            
            if ((dx == 0 && dy == 0) || !(abs(dx) == 0 || abs(dy) == 0)) continue;

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

void GenerateQueenMoves(AppState *app, Piece *p)
{
    app->possibleMoveCount = 0;

    for (int dx = -3; dx <= 3; dx++) {
        for (int dy = -3; dy <= 3; dy++) {
            
            
            if ((dx == 0 && dy == 0) || !(abs(dx) == 0 || abs(dy) == 0) && !(abs(dx) == abs(dy))) continue;

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
        int row = app->possibleMoves[i].y;
        int col = app->possibleMoves[i].x;

        // 🚫 Skip water tiles
        if (!isTileWalkable(row, col)) {
            continue;
        }

        float worldX = col * TILE_SIZE;
        float worldY = row * TILE_SIZE;

        SDL_FRect r = {
            (worldX - app->camera.x) * app->camera.zoom,
            (worldY - app->camera.y) * app->camera.zoom,
            TILE_SIZE * app->camera.zoom,
            TILE_SIZE * app->camera.zoom
        };

        SDL_RenderTexture(app->renderer, app->movePossible, NULL, &r);
    }
}