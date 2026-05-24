#include "piece.h"
#include "../game.h"

void spawnPiece(AppState *app)
{
    if (app->selectedPiece != NULL){return;}
    
    // 1. Check for click and use the dynamic capacity instead of MAX_PIECES
    if (app->input.mouseLeftPressed && app->pieceCount < app->maxPieceCapacity)
    {
        if (pieceSpawnable(app, app->currentPlayer))
        {
            int cost = pieceCost(app->selectedPieceType);
            int *playerGold = (app->currentPlayer == 1) ? &app->P1.p1Gold : &app->P2.p2Gold;

            if (*playerGold < cost)
            {
               //add some indicator here
               app->errorTimer = 1.5f;//UI feedback for not enough gold
                return;
            }

            // 2. Convert screen position to world/grid coordinates
            float worldX = (app->input.mouseX / app->camera.zoom) + app->camera.x;
            float worldY = (app->input.mouseY / app->camera.zoom) + app->camera.y;

            int col = (int)(worldX / TILE_SIZE);
            int row = (int)(worldY / TILE_SIZE);

            // 3. Cheat and Territory checks
            if(!app->cheats && !inTerritory(app, row, col)){ return; }

            // 4. Bounds Check
            if (row >= 0 && row < MAP_ROWS && col >= 0 && col < MAP_COLS)
            {
                char tile = map_data[row][col];

                // 5. Tile Type Validation
                if (tile == GRASS_TILE || tile == BRIDGE_TOP_TILE || tile == BRIDGE_BOTTOM_TILE || tile == UPGRADE_TILE || tile == SPAWN_POINT || tile == TOWN_TILE)
                {
                    float targetX = (float)(col * TILE_SIZE);
                    float targetY = (float)(row * TILE_SIZE);

                    // 6. Occupancy Check using the dynamic capacity
                    bool occupied = false;
                    for (int j = 0; j < app->maxPieceCapacity; j++)
                    {
                        if (app->pieces[j].active &&
                            app->pieces[j].pieceX == targetX &&
                            app->pieces[j].pieceY == targetY)
                        {
                            occupied = true;
                            break;
                        }
                    }

                    if (!occupied)
                    {
                        // 7. Find an empty slot in the dynamic array[cite: 10, 14]
                        for (int i = 0; i < app->maxPieceCapacity; i++)
                        {
                            if (!app->pieces[i].active)
                            {
                                app->pieces[i].pieceX = targetX;
                                app->pieces[i].pieceY = targetY;
                                app->pieces[i].col = col;
                                app->pieces[i].row = row;
                                app->pieces[i].active = true;

                                app->pieces[i].type = app->selectedPieceType;
                                app->pieces[i].owner = app->currentPlayer;

                                // 8. Deduct Gold and Update counts[cite: 14]
                                *playerGold -= cost;
                                app->pieceCount++;

                                //app->selectedPieceType = NULL;
                                if (app->currentPlayer == 1) app->P1.pieceCount++;
                                else app->P2.pieceCount++;

                                break; // Exit after spawning one piece[cite: 14]
                            }
                        }
                    }
                }
            }
        }
    }
}

void renderPiece(AppState *app)
{
    for (int i = 0; i < app->maxPieceCapacity; i++)
    {
        if (app->pieces[i].active)
        {
            // Convert stored world coordinates back to screen pixels
            SDL_FRect p = camera2d_world_to_screen_rect(
                &app->camera,
                app->pieces[i].pieceX,
                app->pieces[i].pieceY,
                TILE_SIZE, TILE_SIZE);

            SDL_Texture *tex = NULL;
            // to swap between textures for 2 players
        if (app->pieces[i].owner == 1)
        {
            switch (app->pieces[i].type)
            {
                case PAWN:
                    tex = app->pieces[i].moved
                        ? app->bluePawnUsedTexture
                        : app->bluePawnTexture;
                    break;

                case KNIGHT:
                    tex = app->pieces[i].moved
                        ? app->blueKnightUsedTexture
                        : app->blueKnightTexture;
                    break;

                case BISHOP:
                    tex = app->pieces[i].moved
                        ? app->blueBishopUsedTexture
                        : app->blueBishopTexture;
                    break;

                case ROOK:
                    tex = app->pieces[i].moved
                        ? app->blueRookUsedTexture
                        : app->blueRookTexture;
                    break;

                case QUEEN:
                    tex = app->pieces[i].moved
                        ? app->blueQueenUsedTexture
                        : app->blueQueenTexture;
                    break;

                case KING:
                    tex = app->pieces[i].moved
                        ? app->blueKingUsedTexture
                        : app->blueKingTexture;
                    break;

                case ENVOY:
                    tex = app->pieces[i].moved
                        ? app->blueEnvoyUsedTexture
                        : app->blueEnvoyTexture;
                    break;

                case LANCER:
                    tex = app->pieces[i].moved
                        ? app->blueLancerUsedTexture
                        : app->blueLancerTexture;
                    break;

                case MAGE:
                    tex = app->pieces[i].moved
                        ? app->blueMageUsedTexture
                        : app->blueMageTexture;
                    break;

                case CATAPULT:
                    tex = app->pieces[i].moved
                        ? app->blueCatapultUsedTexture
                        : app->blueCatapultTexture;
                    break;

                default:
                    tex = app->bluePawnTexture;
                    break;
            }
        }
        else
        {
            switch (app->pieces[i].type)
            {
                case PAWN:
                    tex = app->pieces[i].moved
                        ? app->redPawnUsedTexture
                        : app->redPawnTexture;
                    break;

                case KNIGHT:
                    tex = app->pieces[i].moved
                        ? app->redKnightUsedTexture
                        : app->redKnightTexture;
                    break;

                case BISHOP:
                    tex = app->pieces[i].moved
                        ? app->redBishopUsedTexture
                        : app->redBishopTexture;
                    break;

                case ROOK:
                    tex = app->pieces[i].moved
                        ? app->redRookUsedTexture
                        : app->redRookTexture;
                    break;

                case QUEEN:
                    tex = app->pieces[i].moved
                        ? app->redQueenUsedTexture
                        : app->redQueenTexture;
                    break;

                case KING:
                    tex = app->pieces[i].moved
                        ? app->redKingUsedTexture
                        : app->redKingTexture;
                    break;

                case ENVOY:
                    tex = app->pieces[i].moved
                        ? app->redEnvoyUsedTexture
                        : app->redEnvoyTexture;
                    break;

                case LANCER:
                    tex = app->pieces[i].moved
                        ? app->redLancerUsedTexture
                        : app->redLancerTexture;
                    break;

                case MAGE:
                    tex = app->pieces[i].moved
                        ? app->redMageUsedTexture
                        : app->redMageTexture;
                    break;

                case CATAPULT:
                    tex = app->pieces[i].moved
                        ? app->redCatapultUsedTexture
                        : app->redCatapultTexture;
                    break;

                default:
                    tex = app->redPawnTexture;
                    break;
            }
        }

        if (tex)
        {
            SDL_RenderTexture(app->renderer, tex, NULL, &p);
        }
    }
}
}

bool isTileWalkable(int row, int col)
{
    // 1. Bounds check (ALWAYS first)
    if (row < 0 || row >= MAP_ROWS || col < 0 || col >= MAP_COLS)
    {
        return false;
    }

    // 2. Check the specific tile
    if (map_data[row][col] == 'W' || map_data[row][col] == 'T' || map_data[row][col] == 't' || map_data[row][col] == 'j')
    {
        return false;
    }

    return true;
}

bool pieceOnUpgradeTile(Piece *piece)
{
    if (piece == NULL)
    {
        return false;
    }

    return map_data[piece->row][piece->col] == 'U';
}

bool pieceSpawnable(AppState *app, int player)
{
    if (app->pieceCount >= app->maxPieceCapacity){return false;}
    int count = (player == 1) ? app->P1.pieceCount : app->P2.pieceCount;
    if (count >= app->maxPlayerPieces)return false;

    TechTree *tree = (player == 1) ? &app->techTreeP1 : &app->techTreeP2;
    //add unlock checks here for pieces
    if (app->selectedPieceType == KNIGHT && !tree->upgrades[0].unlocked)//checking if prev node unlocked need update it
    {
        app->errorTimer = 2.0f;
        return false;
    }

    if (app->selectedPieceType == BISHOP && !tree->upgrades[1].unlocked)//checking if prev node unlocked need update it
    {
        app->errorTimer = 2.0f;
        return false;
    }

    if (app->selectedPieceType == ROOK && !tree->upgrades[2].unlocked)//checking if prev node unlocked need update it
    {
        app->errorTimer = 2.0f;
        return false;
    }
    if (app->selectedPieceType == QUEEN && !tree->upgrades[3].unlocked)//checking if prev node unlocked need update it
    {
        app->errorTimer = 2.0f;
        return false;
    }
     if (app->selectedPieceType == ENVOY && !tree->upgrades[5].unlocked)//checking if prev node unlocked need update it
    {
        app->errorTimer = 2.0f;
        return false;
    }
    if (app->selectedPieceType == MAGE && !tree->upgrades[6].unlocked)//checking if prev node unlocked need update it
    {
        app->errorTimer = 2.0f;
        return false;
    }
     if (app->selectedPieceType == CATAPULT && !tree->upgrades[7].unlocked)//checking if prev node unlocked need update it
    {
        app->errorTimer = 2.0f;
        return false;
    }




    if (app->selectedPieceType == KING)return false;

    return true;
}

bool inTerritory(AppState *app, int row, int col){
    for (int t = 0; t < app->tTowns; t++)
    {
        //check who owns town
        if (app->towns[t].owner == app->currentPlayer)
        {
            //abs() check 3x3 radius
            if (abs(row - app->towns[t].row) <= 1 && abs(col - app->towns[t].col) <= 1){return true;}
        } 
    }

    int spawnCount = 0;
    for (int r = 0; r < MAP_ROWS; r++)
    {
        for (int c = 0; c < MAP_COLS; c++)
        {
            if(map_data[r][c] == SPAWN_POINT){
                spawnCount++;

                int spawnOwner = (spawnCount == 1) ? 1 : 2;
                if (spawnOwner == app->currentPlayer)
                {
                    if (abs(row - r) <= 1 && abs(col - c) <= 1){return true;}
                }
                

            }
        }
        
    }
    
    return false;
    
}

int pieceCost(pieceType type){
    switch(type){
        case PAWN: return 1;break;
        case KNIGHT:return 2;break;
        case BISHOP:return 4;break;
        case ROOK:return 3;break;
        case QUEEN:return 6;break;
        case ENVOY: return 4;break;
        case LANCER: return 5;break;
        case MAGE: return 7;break;
        case CATAPULT: return 9;break;
        default: return 0;
    }
}

int countPiecesByType(AppState *app, int player, pieceType type) {
    int total = 0;
    for (int i = 0; i < app->maxPieceCapacity; i++) {
        if (app->pieces[i].active && app->pieces[i].owner == player && app->pieces[i].type == type) {
            total++;
        }
    }
    return total;
}

bool pieceUnlocked(AppState *app, pieceType type)
{
    TechTree *tree;

    if (app->currentPlayer == 1)
        tree = &app->techTreeP1;
    else
        tree = &app->techTreeP2;

    switch(type)
    {
        // base units always unlocked
        case PAWN:
        case KING:
            return true;

        case KNIGHT:
            return tree->upgrades[0].unlocked;

        case BISHOP:
            return tree->upgrades[5].unlocked;

        case ROOK:
            return tree->upgrades[4].unlocked;

        case QUEEN:
            return tree->upgrades[9].unlocked;

        case ENVOY:
            return tree->upgrades[1].unlocked;

        case MAGE:
            return tree->upgrades[7].unlocked;

        case CATAPULT:
            return tree->upgrades[8].unlocked;

        case LANCER:
            return tree->upgrades[6].unlocked;

        default:
            return false;
    }
}

bool pieceCountReached(AppState *app, pieceType type)
{
    int player =
        app->currentPlayer;

    int count =
        countPiecesByType(app, player, type);

    int towns =
        (player == 1)
        ? app->P1.towns + 1
        : app->P2.towns + 1;

    int maxAllowed = 0;

    switch(type)
    {
        case PAWN:
            maxAllowed = towns * 8;
            break;

        case KNIGHT:
            maxAllowed = towns * 2;
            break;

        case BISHOP:
            maxAllowed = towns * 2;
            break;

        case ROOK:
            maxAllowed = towns * 2;
            break;

        case QUEEN:
            maxAllowed = towns;
            break;

        case ENVOY:
            maxAllowed = towns * 2;
            break;

        case LANCER:
            maxAllowed = towns * 2;
            break;

        case MAGE:
            maxAllowed = towns * 2;
            break;

        case CATAPULT:
            maxAllowed = towns * 1;
            break;

        default:
            return false;
    }

    return count >= maxAllowed;
}

pieceType getUpgradeType(pieceType type)
{
    switch(type)
    {
        case PAWN:
            return ENVOY;

        case KNIGHT:
            return LANCER;

        case BISHOP:
            return MAGE;

        case ROOK:
            return CATAPULT;

        default:
    }
}




pieceType rollUpgradePlatformPiece(AppState *app)
{
    TechTree *tree =
        (app->currentPlayer == 1)
        ? &app->techTreeP1
        : &app->techTreeP2;

    int roll = rand() % 100;

    switch(tree->upgradePlatformLevel)
    {
        case 1:

            // Pawn 50%
            if (roll < 50) return PAWN;

            // Knight 25%
            if (roll < 75) return KNIGHT;

            // Bishop 13%
            if (roll < 88) return BISHOP;

            // Envoy 12%
            return ENVOY;

        case 2:

            // Pawn 35%
            if (roll < 35) return PAWN;

            // Knight 25%
            if (roll < 60) return KNIGHT;

            // Bishop 15%
            if (roll < 75) return BISHOP;

            // Envoy 10%
            if (roll < 85) return ENVOY;

            // Lancer 7%
            if (roll < 92) return LANCER;

            // Mage 5%
            if (roll < 97) return MAGE;

            // Rook 3%
            return ROOK;

        case 3:

            // Pawn 20%
            if (roll < 20) return PAWN;

            // Knight 17%
            if (roll < 37) return KNIGHT;

            // Bishop 12%
            if (roll < 49) return BISHOP;

            // Rook 15%
            if (roll < 64) return ROOK;

            // Envoy 8%
            if (roll < 72) return ENVOY;

            // Lancer 10%
            if (roll < 82) return LANCER;

            // Mage 8%
            if (roll < 90) return MAGE;

            // Catapult 7%
            if (roll < 97) return CATAPULT;

            // Queen 3%
            return QUEEN;
    }

    return PAWN;
}

void useUpgradePlatform(AppState *app, Piece *piece)
{
    if (!piece || piece->type == KING || piece->abilityUsed) return;
    if (!piece->active) return;

    TechTree *tree =
        (app->currentPlayer == 1)
        ? &app->techTreeP1
        : &app->techTreeP2;

    // Need at least level 1
    if (tree->upgradePlatformLevel <= 0) return;

    pieceType newType =
        rollUpgradePlatformPiece(app);

    piece->type = newType;
    piece->abilityUsed = true;
    piece->moved = true;
    app->selectedPiece = NULL;

    // Consumes move
    piece->moved = true;

    printf("Upgrade Platform rolled: %d\n", newType);
}