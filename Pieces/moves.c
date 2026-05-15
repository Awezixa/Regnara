#include "../game.h"
#include "piece.h"
#include "moves.h"

void GenerateMoves(AppState *app, Piece *p)
{
    switch (p->type)
    {
        case PAWN:GenerateKingMoves(app, p);break;
        case KNIGHT:GenerateKnightMoves(app,p);break;
        case BISHOP:GenerateBishopMoves(app,p);break;
        case ROOK:GenerateRookMoves(app,p);break;
        case QUEEN: GenerateQueenMoves(app,p);break;
        case KING:GenerateKingMoves(app, p);break;
        case ENVOY: GenerateKingMoves(app, p);break;
        case LANCER:
                    GenerateLancerMoves(app, p);break;
        case MAGE:
                    GenerateBishopMoves(app, p);
                    GenerateMageAttacks(app, p);
                    break;

        case CATAPULT:
                    GenerateCatapultMoves(app, p);
                    GenerateCatapultAttacks(app, p);
                    break;
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
            
            
            if (dx == 0 || dy == 0 || (abs(dx) == 1 && abs(dy) == 1) || (abs(dx) == 2 && abs(dy) == 2)) continue;

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
            
            
            if ((dx == 0 && dy == 0) || !(abs(dx) == abs(dy))) continue;

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

void GenerateLancerMoves(AppState *app, Piece *p)
{
    GenerateKnightMoves(app, p);
}

void GenerateMageAttacks(AppState *app, Piece *p)
{
    app->possibleAttackCount = 0;

    const int attacks[4][2] = {

        { 0,-3},
        { 0, 3},
        {-3, 0},
        { 3, 0}
    };

    for (int i = 0; i < 4; i++)
    {
        int newCol = p->col + attacks[i][0];
        int newRow = p->row + attacks[i][1];

        if (newCol < 0 || newCol >= MAP_COLS ||
            newRow < 0 || newRow >= MAP_ROWS)
            continue;

        app->possibleAttacks[app->possibleAttackCount++] =
            (SDL_Point){newCol, newRow};
    }
}

void GenerateCatapultMoves(AppState *app, Piece *p)
{
    app->possibleMoveCount = 0;

    // rook-style movement up to 2 tiles
    const int moves[8][2] = {

        {-2,  0},
        {-1,  0},
        { 1,  0},
        { 2,  0},

        { 0, -2},
        { 0, -1},
        { 0,  1},
        { 0,  2}
    };

    for (int i = 0; i < 8; i++)
    {
        int newCol = p->col + moves[i][0];
        int newRow = p->row + moves[i][1];

        // bounds check
        if (newCol < 0 || newCol >= MAP_COLS ||
            newRow < 0 || newRow >= MAP_ROWS)
            continue;

        app->possibleMoves[app->possibleMoveCount++] =
            (SDL_Point){newCol, newRow};
    }
}

void GenerateCatapultAttacks(AppState *app, Piece *p)
{
    app->possibleAttackCount = 0;

    const int attacks[8][2] = {

        {-3,-1},
        {-3, 1},
        { 3,-1},
        { 3, 1},

        {-1,-3},
        { 1,-3},
        {-1, 3},
        { 1, 3}
    };

    for (int i = 0; i < 8; i++)
    {
        int newCol = p->col + attacks[i][0];
        int newRow = p->row + attacks[i][1];

        if (newCol < 0 || newCol >= MAP_COLS ||
            newRow < 0 || newRow >= MAP_ROWS)
            continue;

        app->possibleAttacks[app->possibleAttackCount++] =
            (SDL_Point){newCol, newRow};
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

                Piece *target = GetPieceAt(app, row, col);

        // 🚫 Skip friendly pieces
        if (target && target->owner == p->owner) {
            continue;
        }

        // default texture
        SDL_Texture *tex = app->movePossible;

        // =========================
        // NORMAL CAPTURE
        // =========================
        if (target && target->owner != p->owner)
        {
            tex = app->moveCapture;

            // =========================
            // LANCER PIERCE PREVIEW
            // =========================
            if (p->type == LANCER)
            {
                int dx = col - p->col;
                int dy = row - p->row;

                int pierceCol = col;
                int pierceRow = row;

                // horizontal attack
                if (abs(dx) > abs(dy))
                {
                    if (dx > 0)
                        pierceCol += 1;
                    else
                        pierceCol -= 1;
                }
                // vertical attack
                else
                {
                    if (dy > 0)
                        pierceRow += 1;
                    else
                        pierceRow -= 1;
                }

                // bounds check
                if (pierceCol >= 0 && pierceCol < MAP_COLS &&
                    pierceRow >= 0 && pierceRow < MAP_ROWS)
                {
                    Piece *behind = GetPieceAt(app, pierceRow, pierceCol);

                    // show capture texture on pierced enemy
                    if (behind && behind->owner != p->owner)
                    {
                        float behindWorldX = pierceCol * TILE_SIZE;
                        float behindWorldY = pierceRow * TILE_SIZE;

                        SDL_FRect pierceRect = {
                            (behindWorldX - app->camera.x) * app->camera.zoom,
                            (behindWorldY - app->camera.y) * app->camera.zoom,
                            TILE_SIZE * app->camera.zoom,
                            TILE_SIZE * app->camera.zoom
                        };

                        SDL_RenderTexture(app->renderer,
                                        app->moveLancer,
                                        NULL,
                                        &pierceRect);
                    }
                }
            }
        }

        SDL_RenderTexture(app->renderer, tex, NULL, &r);
    }

    for (int i = 0; i < app->possibleAttackCount; i++)
    {
        int col = app->possibleAttacks[i].x;
        int row = app->possibleAttacks[i].y;

        // bounds/walkable check
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

        Piece *target = GetPieceAt(app, row, col);

        // only show ranged attack if enemy exists there
        if (target && target->owner != p->owner)
        {
            SDL_RenderTexture(app->renderer, app->moveRanged, NULL, &r);
        }
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

void LancerAttack(AppState *app, Piece *attacker, int targetRow, int targetCol)
{
    // capture original target
    CapturePiece(app, targetRow, targetCol);

    int dx = targetCol - attacker->col;
    int dy = targetRow - attacker->row;

    int pierceCol = targetCol;
    int pierceRow = targetRow;

    // horizontal knight movement
    if (abs(dx) > abs(dy))
    {
        if (dx > 0)
            pierceCol += 1;
        else
            pierceCol -= 1;
    }
    // vertical knight movement
    else
    {
        if (dy > 0)
            pierceRow += 1;
        else
            pierceRow -= 1;
    }

    // bounds check
    if (pierceCol < 0 || pierceCol >= MAP_COLS ||
        pierceRow < 0 || pierceRow >= MAP_ROWS)
    {
        return;
    }

    Piece *behind = GetPieceAt(app, pierceRow, pierceCol);

    // only capture enemy
    if (behind && behind->owner != attacker->owner)
    {
        CapturePiece(app, pierceRow, pierceCol);
    }
}

void CatapultAttack(AppState *app, int row, int col)
{
    CapturePiece(app, row, col);

    const int splash[4][2] = {
        {-1, 0},
        { 1, 0},
        { 0,-1},
        { 0, 1}
    };

    for (int i = 0; i < 4; i++)
    {
        int splashCol = col + splash[i][0];
        int splashRow = row + splash[i][1];

        if (splashCol < 0 || splashCol >= MAP_COLS ||
            splashRow < 0 || splashRow >= MAP_ROWS)
            continue;

        Piece *p = GetPieceAt(app, splashRow, splashCol);
        
        if (p && p->owner != app->currentPlayer)
        {
            CapturePiece(app, splashRow, splashCol);
        }
    }
    if (app->selectedPiece->abilityUsed)
        return;
        app->selectedPiece->abilityUsed = true;
}

