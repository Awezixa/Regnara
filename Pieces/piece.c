#include "piece.h"
#include "../game.h"

void spawnPiece(AppState *app)
{
    // change made to convert mouse screen position to world mouse position, to prevent pieces spawning wrong place
    if (app->input.mouseLeftPressed && app->pieceCount < MAX_PIECES)
    {
        // need to add check for map data to only allow spawn towns and area around towns
        float worldX = (app->input.mouseX / app->camera.zoom) + app->camera.x;
        float worldY = (app->input.mouseY / app->camera.zoom) + app->camera.y;

        int col = (int)(worldX / TILE_SIZE); // world units to grid loactions
        int row = (int)(worldY / TILE_SIZE);

        if (row >= 0 && row < MAP_ROWS && col >= 0 && col < MAP_COLS)
        {
            char tile = map_data[row][col];
            for (int i = 0; i < MAX_PIECES; i++)
            {
                if (tile == GRASS_TILE || tile == BRIDGE_TILE || tile == SPAWN_POINT)
                {
                    if (!app->pieces[i].active)
                    {
                        // CONVERT SCREEN TO WORLD:
                        // Math: (ScreenPos / Zoom) + CameraOffset
                        app->pieces[i].pieceX = (float)(col * TILE_SIZE);
                        app->pieces[i].pieceY = (float)(row * TILE_SIZE);

                        app->pieces[i].active = true;
                        app->pieceCount++;
                        break;
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
    int cellSize = TILE_SIZE;

    for (int i = 0; i < app->pieceCount; i++)
    {
        Piece *p = &app->pieces[i];

        SDL_FRect rect = {
            p->pieceX * cellSize,
            p->pieceY * cellSize,
            cellSize,
            cellSize
        };

        SDL_Texture *tex = app->WkingTexture; // temp

        SDL_RenderTexture(app->renderer, tex, NULL, &rect);
    }
}

SDL_FRect GetPieceRect(Piece *p, int cellSize)
{
    return (SDL_FRect){
        p->pieceX * cellSize,
        p->pieceY * cellSize,
        cellSize,
        cellSize
    };
}
