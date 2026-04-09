#include "piece.h"
#include "../game.h"

void spawnPiece(AppState *app)
{
    // 1. Initial check for click and capacity
    if (app->input.mouseLeftPressed && app->pieceCount < MAX_PIECES)
    {
        // 2. Refresh relative mouse state to distinguish click from drag
        float relX, relY;
        SDL_GetGlobalMouseState(NULL, NULL); 
        SDL_GetRelativeMouseState(&relX, &relY);

        // Only proceed if the mouse stayed relatively still (not a camera drag)
        if (SDL_fabsf(relX) < 2.0f && SDL_fabsf(relY) < 2.0f)
        {
            // 3. Convert screen position to world/grid coordinates
            float worldX = (app->input.mouseX / app->camera.zoom) + app->camera.x;
            float worldY = (app->input.mouseY / app->camera.zoom) + app->camera.y;

            int col = (int)(worldX / TILE_SIZE);
            int row = (int)(worldY / TILE_SIZE);

            // 4. Bounds Check
            if (row >= 0 && row < MAP_ROWS && col >= 0 && col < MAP_COLS)
            {
                char tile = map_data[row][col];

                // 5. Tile Type Validation
                if (tile == GRASS_TILE || tile == BRIDGE_TILE || tile == SPAWN_POINT)
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

                                // FIX: Use selectedPieceType (enum) not selectedPiece (pointer)
                                app->pieces[i].type = app->selectedPieceType;

                                app->pieceCount++;
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

void renderPiece(AppState *app) {
    for (int i = 0; i < MAX_PIECES; i++)
    {
        if (app->pieces[i].active)
        {
            // Convert stored world coordinates back to screen pixels
            SDL_FRect p = camera2d_world_to_screen_rect(
                &app->camera,
                app->pieces[i].pieceX,
                app->pieces[i].pieceY,
                32.0f, 32.0f 
            );

            SDL_Texture *tex = NULL;
            switch (app->pieces[i].type)
            {
                case PAWN:   tex = app->WpawnTexture;   break;
                case KING:   tex = app->WkingTexture;   break;
                case QUEEN:  tex = app->WqueenTexture;  break;
                case ROOK:   tex = app->WrookTexture;   break;
                case KNIGHT: tex = app->WknightTexture; break;
                case BISHOP: tex = app->WbishopTexture; break;
                default: break;
            }

            if (tex) {
                SDL_RenderTexture(app->renderer, tex, NULL, &p);
            }
        }
    }
}