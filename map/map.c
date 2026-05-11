#include "../game.h"
#include "map.h"
#include <stdio.h>


char map_data[MAP_ROWS][MAP_COLS];

/*
can make map in Tiled then export as CSV.
replace commas with spaces and then associate csv ids to what you want and then load map that way
*/

// Xavier with Cesar's help
void loadMap(const char *filename)
{
    FILE *file = fopen(filename, "r");

    if (!file)
    {
        printf("Failed to open map file: %s\n", filename);
        return;
    }

    printf("Loading map %s\n", filename);

    for (int row = 0; row < MAP_ROWS; row++)
    {
        for (int col = 0; col < MAP_COLS;)
        {
            int ch = fgetc(file);

            if (ch == EOF)
            {
                printf("Unexpected EOF in map file\n");
                fclose(file);
                return;
            }

            // Skip Windows + Unix line endings
            if (ch == '\n' || ch == '\r')
            {
                continue;
            }

            map_data[row][col] = (char)ch;
            col++;
        }
    }

    fclose(file);
}

//made with Gemini (Xavier), ask Cesar if it ok
void renderMap(SDL_Renderer *renderer, AppState *app) {
    for (int row = 0; row < MAP_ROWS; row++) {
        for (int col = 0; col < MAP_COLS; col++) {
            SDL_FRect destRect = camera2d_world_to_screen_rect(
                &app->camera,
                (float)(col * TILE_SIZE), 
                (float)(row * TILE_SIZE), 
                (float)TILE_SIZE, 
                (float)TILE_SIZE
            );

            SDL_Texture *targetTexture = NULL;

            // Map the character in the txt file to the loaded texture
            switch (map_data[row][col]) {
                case 'G': targetTexture = app->grassTexture; break;
                case 'W': targetTexture = app->waterTexture; break;
                case 'B': targetTexture = app->bridgeTopTexture; break;
                case 'b': targetTexture = app->bridgeBottomTexture; break;
                case 'C': targetTexture = app->townTexture; break;
                case 'S': targetTexture = app->townTexture; break;
                case 'U': targetTexture = app->upgradePlatformTexture; break;

                case 'T': targetTexture = app->treeTexture; break;
                case 't': targetTexture = app->treeRightTexture; break;
                case 'j': targetTexture = app->treeLeftTexture; break;

                case '1': targetTexture = app->shoreLeftTexture; break;
                case '2': targetTexture = app->shoreRightTexture; break;
                case '3': targetTexture = app->shoreTopTexture; break;
                case '4': targetTexture = app->shoreBottomTexture; break;

                case '5': targetTexture = app->shoreBottomLeftTexture; break;
                case '6': targetTexture = app->shoreBottomRightTexture; break;
                case '7': targetTexture = app->shoreTopLeftTexture; break;
                case '8': targetTexture = app->shoreTopRightTexture; break;

                case '9': targetTexture = app->shoreCornerTopLeftTexture; break;
                case '0': targetTexture = app->shoreCornerTopRightTexture; break;
                case '#': targetTexture = app->shoreCornerBottomLeftTexture; break;
                case '*': targetTexture = app->shoreCornerBottomRightTexture; break;

                default: break; 
            }

            if (targetTexture) {
                SDL_RenderTexture(renderer, targetTexture, NULL, &destRect);
            }

            //to create grid texture to map
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 50);
            SDL_RenderRect(renderer, &destRect);
        }
    }
}


int townCount(void){
    int total = 0;
    for(int row = 0; row < MAP_ROWS; row++){
        for (int col = 0; col < MAP_COLS; col++){
           if (map_data[row][col] == 'C') {
                total++;
           } 
        }
    }
    return total;
}


void initTowns(AppState *app){
    app->tTowns = 0;
    for (int r = 0; r < MAP_ROWS; r++) {
        for (int c = 0; c < MAP_COLS; c++) {
            if (map_data[r][c] == 'C') {
                app->towns[app->tTowns].row = r;
                app->towns[app->tTowns].col = c;
                app->towns[app->tTowns].owner = 0; // Neutral
                app->towns[app->tTowns].captureTurns = 0;
                app->tTowns++;
            }
        }
    }
}

void townCaptured(AppState *app) {
    for (int t = 0; t < app->tTowns; t++) {
        Town *town = &app->towns[t];
        bool kingPresent = false;
        int kingOwner = 0;

        // 1. Search for a King piece specifically at this town's location
        for (int i = 0; i < app->maxPieceCapacity; i++) {
            Piece *p = &app->pieces[i];
            
            // CRITICAL: Check the actual owner field of the piece
            if (p->active && p->type == KING && p->row == town->row && p->col == town->col) {
                kingPresent = true;
                kingOwner = p->owner; // Use the piece's stored owner
                break;
            }
        }

        // 2. If a King is present and it's not the current town owner
        if (kingPresent && town->owner != kingOwner) {
            town->captureTurns++;
            
            if (town->captureTurns >= 1) { // Captured after 1 turn
                int oldOwner = town->owner;
                town->owner = kingOwner;
                town->captureTurns = 0;

                // 3. Update persistent player town counts accurately
                if (kingOwner == 1) app->P1.towns++;
                else if (kingOwner == 2) app->P2.towns++;

                // 4. Subtract from previous owner if it wasn't neutral
                if (oldOwner == 1) app->P1.towns--;
                else if (oldOwner == 2) app->P2.towns--;
            }
        } else {
            // Reset if the King moves away or if no King is present
            town->captureTurns = 0;
        }
    }
}

void drawTerritory(AppState *app) {
    // Set the blend mode once for the whole loop
    SDL_SetRenderDrawBlendMode(app->renderer, SDL_BLENDMODE_BLEND);

    int spawnCount = 0;

    for (int t = 0; t < app->tTowns; t++) {
        Town *town = &app->towns[t];

        if (town->owner != 0) {
            // Set Color: Blue for P1, Red for P2
            if (town->owner == 1) {
                SDL_SetRenderDrawColor(app->renderer, 0, 100, 255, 80); 
            } else {
                SDL_SetRenderDrawColor(app->renderer, 255, 50, 50, 80);
            }

            // Loop through the 3x3 radius (-1, 0, 1)
            for (int dr = -1; dr <= 1; dr++) {
                for (int dc = -1; dc <= 1; dc++) {
                    // CRITICAL: Offset from the actual town position
                    int r = town->row + dr;
                    int c = town->col + dc;

                    // 3. Bounds check to ensure we stay on the map
                    if (r >= 0 && r < MAP_ROWS && c >= 0 && c < MAP_COLS) {
                        // 4. Convert World Grid to Screen Rect
                        SDL_FRect tileRect = camera2d_world_to_screen_rect(
                            &app->camera,
                            (float)(c * TILE_SIZE),
                            (float)(r * TILE_SIZE),
                            (float)TILE_SIZE,
                            (float)TILE_SIZE
                        );
                        SDL_RenderFillRect(app->renderer, &tileRect);
                    }
                }
            }
        }
    }

    //spawn territory rendering
    for (int i = 0; i < MAP_ROWS; i++)
    {
        for (int j = 0; j < MAP_COLS; j++)
        {
            if(map_data[i][j]== SPAWN_POINT)
            {
                spawnCount++;

                if (spawnCount == 1) {
                    SDL_SetRenderDrawColor(app->renderer, 0, 100, 255, 80); 
                } else {
                    SDL_SetRenderDrawColor(app->renderer, 255, 50, 50, 80);
                }

                //to draw square
                for (int dr = -1; dr <= 1; dr++)
                {
                    for (int dc = -1; dc <= 1; dc++)
                    {
                        //tile row and col
                        int tr = i + dr;
                        int tc = j + dc;

                        if (tr >= 0 && tr < MAP_ROWS && tc >= 0 && tc < MAP_COLS) {
                            SDL_FRect tileRect = camera2d_world_to_screen_rect(&app->camera, (float)(tc * TILE_SIZE), (float)(tr * TILE_SIZE), (float)TILE_SIZE, (float)TILE_SIZE);
                            SDL_RenderFillRect(app->renderer, &tileRect);
                            
                        }
                    }

                }
            }
        
        }
    }
}