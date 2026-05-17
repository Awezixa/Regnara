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

// back up spawn piece if original breaks

// void spawnPiece(AppState *app)
// {
//     // 1. Only run if a click happened and we have room for more pieces
//     if (app->input.mouseLeftPressed && app->pieceCount < MAX_PIECES)
//     {
//         // 2. Convert Mouse Screen Position to World Coordinates
//         float worldX = (app->input.mouseX / app->camera.zoom) + app->camera.x;
//         float worldY = (app->input.mouseY / app->camera.zoom) + app->camera.y;

//         // 3. Convert World Units to Grid Indices
//         // col corresponds to X (horizontal), row corresponds to Y (vertical)
//         int col = (int)(worldX / TILE_SIZE);
//         int row = (int)(worldY / TILE_SIZE);

//         // 4. Bounds Check: Ensure the click is actually on the map
//         if (row >= 0 && row < MAP_ROWS && col >= 0 && col < MAP_COLS)
//         {
//             // 5. Tile Check: Use map_data[row][col] correctly
//             char tile = map_data[row][col];
//             if (tile == GRASS_TILE || tile == BRIDGE_TILE || tile == SPAWN_POINT)
//             {
//                 // 6. Find the first available slot in the piece array
//                 for (int i = 0; i < MAX_PIECES; i++)
//                 {
//                     if (!app->pieces[i].active)
//                     {
//                         // 7. Snap to Grid: Set piece world position to the tile's top-left
//                         app->pieces[i].pieceX = (float)(col * TILE_SIZE);
//                         app->pieces[i].pieceY = (float)(row * TILE_SIZE);

//                         app->pieces[i].active = true;
//                         app->pieceCount++;
//                         break; // Exit the loop after spawning one piece
//                     }
//                 }
//             }
//         }
//     }
// }

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
                    case PAWN:tex = app->bluePawnTexture;break;
                    case KNIGHT:tex = app->blueKnightTexture;break;
                    case BISHOP:tex = app->blueBishopTexture;break;
                    case ROOK:tex = app->blueRookTexture;break;
                    case QUEEN:tex = app->blueQueenTexture;break;
                    case KING:tex = app->blueKingTexture;break;
                    case ENVOY:tex = app->blueEnvoyTexture;break;
                    case LANCER: tex = app->blueLancerTexture; break;
                    case MAGE: tex = app->blueMageTexture; break;
                    case CATAPULT: tex = app->blueCatapultTexture; break;
                    default: tex = app->bluePawnTexture; break;
                }
            }
            else
            {
                switch (app->pieces[i].type)
                {
                    case PAWN:tex = app->redPawnTexture;break;
                    case KNIGHT:tex = app->redKnightTexture;break;
                    case BISHOP:tex = app->redBishopTexture;break;
                    case ROOK:tex = app->redRookTexture;break;
                    case QUEEN:tex = app->redQueenTexture;break;
                    case KING:tex = app->redKingTexture;break;
                    case ENVOY:tex = app->redEnvoyTexture;break;
                    case LANCER: tex = app->redLancerTexture; break;
                    case MAGE: tex = app->redMageTexture; break;
                    case CATAPULT: tex = app->redCatapultTexture; break;
                    default: tex = app->redPawnTexture;break;
                }
            }

            if (tex)
            {

                if (app->pieces[i].moved == true)
                {
                    SDL_SetTextureAlphaMod(tex, 110);
                }
                else{
                    SDL_SetTextureAlphaMod(tex, 255);
                }
                
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
    return true;
}

bool pieceCountReached(AppState *app, pieceType type)
{
    return false;
}