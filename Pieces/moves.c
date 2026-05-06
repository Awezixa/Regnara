#include "../game.h"
#include "piece.h"
#include "moves.h"

void GenerateMoves(AppState *app, Piece *p)
{
    switch (p->type)
    {
        case KING:GenerateKingMoves(app, p);break;
        case KNIGHT:GenerateKnightMoves(app,p);break;
        case BISHOP:GenerateBishopMoves(app,p);break;
        case ROOK:GenerateRookMoves(app,p);break;
        case QUEEN: GenerateQueenMoves(app,p);break;
        case PAWN:GenerateKingMoves(app, p);break;
    }
}

bool isTileOccupied(AppState *app, int row, int col)
{
    for (int i = 0; i < app->maxPieceCapacity; i++)
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
    for (int i = 0; i < app->maxPieceCapacity; i++)
    {
        Piece *other = &app->pieces[i];

        if (other->active && other->row == row && other->col == col)
        {
            // SAFETY: If the piece we are capturing is currently selected, deselect it
            if (app->selectedPiece == other) {
                app->selectedPiece = NULL;
                app->possibleMoveCount = 0;
            }

            if (other->type == KING) {
                app->winner = (other->owner == 1) ? 2 : 1;
                app->gameState = STATE_END;
            }
            
            if(other->owner == 1) app->P1.pieceCount--;
            else if(other->owner == 2) app->P2.pieceCount--;
            
            app->pieceCount--;
            other->active = false; 
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
        int col = app->possibleMoves[i].x;
        int row = app->possibleMoves[i].y;

        // 🚫 Skip water
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

        // 🔍 Check if there's a piece here
        Piece *target = GetPieceAt(app, row, col);

        // 🚫 Skip friendly pieces
        if (target && target->owner == p->owner) {
            continue;
        }

        // 🎨 Choose texture
        SDL_Texture *tex = app->movePossible;

        if (target && target->owner != p->owner) {
            tex = app->moveCapture;
        }

        SDL_RenderTexture(app->renderer, tex, NULL, &r);
    }
}

Piece* GetPieceAt(AppState *app, int row, int col)
{
    for (int i = 0; i < app->maxPieceCapacity; i++)
    {
        Piece *p = &app->pieces[i];

        if (p->active && p->row == row && p->col == col)
        {
            return p;
        }
    }
    return NULL;
}