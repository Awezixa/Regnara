#include "piece.h"
#include "../game.h"

void spawnPiece(AppState *app)
{
    // 1. Initial check for click and capacity
    if (app->input.mouseRightPressed && app->pieceCount < MAX_PIECES)
    {
        if (pieceSpawnable(app, app->currentPlayer))
        {

            int cost = pieceCost(app->selectedPieceType);
            int *playerGold = (app->currentPlayer == 1) ? &app->P1.p1Gold : &app->P2.p2Gold;

            if (*playerGold < cost)
            {
                //add temp text that show player cant spawn
                SDL_Log("Not enough gold");
                return;
            }
            

            // 2. Refresh relative mouse state to distinguish click from drag
            float relX, relY;
            SDL_GetGlobalMouseState(NULL, NULL);
            SDL_GetRelativeMouseState(&relX, &relY);

            // 3. Convert screen position to world/grid coordinates
            float worldX = (app->input.mouseX / app->camera.zoom) + app->camera.x;
            float worldY = (app->input.mouseY / app->camera.zoom) + app->camera.y;

            int col = (int)(worldX / TILE_SIZE);
            int row = (int)(worldY / TILE_SIZE);

            if(!app->cheats && !inTerritory(app, row, col)){return;}


            // 4. Bounds Check
            if (row >= 0 && row < MAP_ROWS && col >= 0 && col < MAP_COLS)
            {
                char tile = map_data[row][col];

                // 5. Tile Type Validation
                if (tile == GRASS_TILE || tile == BRIDGE_TILE || tile == SPAWN_POINT || tile == TOWN_TILE)
                {
                    float targetX = (float)(col * TILE_SIZE);
                    float targetY = (float)(row * TILE_SIZE);

                    // 6. Occupancy Check (is there already a piece here?)
                    bool occupied = false;
                    for (int j = 0; j < MAX_PIECES; j++)
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
                        // 7. Find an empty slot and spawn
                        for (int i = 0; i < MAX_PIECES; i++)
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

                                *playerGold -= cost;
                                
                                app->pieceCount++;

                                if (app->currentPlayer == 1)
                                {
                                    app->P1.pieceCount++;
                                }
                                else
                                {
                                    app->P2.pieceCount++;
                                }

                                break;
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
    for (int i = 0; i < MAX_PIECES; i++)
    {
        if (app->pieces[i].active)
        {
            // Convert stored world coordinates back to screen pixels
            SDL_FRect p = camera2d_world_to_screen_rect(
                &app->camera,
                app->pieces[i].pieceX,
                app->pieces[i].pieceY,
                32.0f, 32.0f);

            SDL_Texture *tex = NULL;
            // to swap between textures for 2 players
            if (app->pieces[i].owner == 1)
            {
                switch (app->pieces[i].type)
                {
                    case PAWN:tex = app->WpawnTexture;break;
                    case KING:tex = app->WkingTexture;break;
                    case QUEEN:tex = app->WqueenTexture;break;
                    case ROOK:tex = app->WrookTexture;break;
                    case KNIGHT:tex = app->WknightTexture;break;
                    case BISHOP:tex = app->WbishopTexture;break;
                    default: tex = app->WpawnTexture; break;
                }
            }
            else
            {
                switch (app->pieces[i].type)
                {
                    case PAWN:tex = app->BpawnTexture;break;
                    case KING:tex = app->BkingTexture;break;
                    case QUEEN:tex = app->BqueenTexture;break;
                    case ROOK:tex = app->BrookTexture;break;
                    case KNIGHT:tex = app->BknightTexture;break;
                    case BISHOP:tex = app->BbishopTexture;break;
                    default: tex = app->BpawnTexture;break;
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
    if (map_data[row][col] == 'W')
    {
        return false;
    }

    return true;
}

bool pieceSpawnable(AppState *app, int player)
{
    if (app->pieceCount >= MAX_PIECES)
    {
        return false;
    }
    int count = (player == 1) ? app->P1.pieceCount : app->P2.pieceCount;

    if (count >= 9)
    {
        return false;
    }

    if (app->selectedPieceType == KING)
    {
        return false;
    }

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
        case QUEEN:return 6;break;
        case ROOK:return 3;break;
        case KNIGHT:return 2;break;
        case BISHOP:return 4;break;
        default: return 0;
    }
}