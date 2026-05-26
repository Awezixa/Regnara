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


void initTowns(AppState *app) {
    app->tTowns = 0;
    int spawnFound = 0;

    app->P1.towns = 0;
    app->P2.towns = 0;

    for (int r = 0; r < MAP_ROWS; r++) {
        for (int c = 0; c < MAP_COLS; c++) {
            char tile = map_data[r][c];

            if (tile == 'C' || tile == 'S') {
                // Use the new higher limit
                if (app->tTowns >= 24) break; 

                app->towns[app->tTowns].row = r;
                app->towns[app->tTowns].col = c;
                app->towns[app->tTowns].captureTurns = 0;

                if (tile == 'S') {
                    spawnFound++;
                    // This logic assumes the FIRST 'S' found is P1, 
                    // and the SECOND 'S' found is P2.
                    int owner = (spawnFound == 1) ? 1 : 2;
                    app->towns[app->tTowns].owner = owner;

                    if (owner == 1) app->P1.towns++;
                    else if (owner == 2) app->P2.towns++;
                } else {
                    app->towns[app->tTowns].owner = 0; // Neutral Town
                }
                app->tTowns++;
            }
        }
    }
}

void townCaptured(AppState *app) {
    for (int t = 0; t < app->tTowns; t++) {
        Town *town = &app->towns[t];
        int capturingOwner = 0;
        int requiredTurns = 0;

        //Check for a King or Envoy at the town's location
        for (int i = 0; i < app->maxPieceCapacity; i++) {
            Piece *p = &app->pieces[i];
            
            if (p->active && p->row == town->row && p->col == town->col) {
                if (p->type == KING) {
                    capturingOwner = p->owner;
                    requiredTurns = 1; 
                    break; 
                } else if (p->type == ENVOY) {
                    capturingOwner = p->owner;
                    requiredTurns = 3; 
                    break;
                }
            }
        }

        // Process Capture Logic
        // Only increment if someone is there and they don't already own it
        if (capturingOwner != 0 && town->owner != capturingOwner) {
            town->captureTurns++;
            
            if (town->captureTurns >= requiredTurns) {
                int oldOwner = town->owner;
                town->owner = capturingOwner;
                town->captureTurns = 0;

                // Update player town counts
                if (capturingOwner == 1) app->P1.towns++;
                else if (capturingOwner == 2) app->P2.towns++;

                // Subtract from previous owner
                if (oldOwner == 1) app->P1.towns--;
                else if (oldOwner == 2) app->P2.towns--;
            }
        } else {
            // Reset if the tile is empty or the current owner is standing on it
            town->captureTurns = 0;
        }
    }
}


void drawTerritory(AppState *app) {
    SDL_SetRenderDrawBlendMode(app->renderer, SDL_BLENDMODE_BLEND);

    // This loop now handles both Towns and Captured Spawn Points
    for (int t = 0; t < app->tTowns; t++) {
        Town *town = &app->towns[t];

        if (town->owner != 0) {
            // Blue for P1, Red for P2
            if (town->owner == 1) {
                SDL_SetRenderDrawColor(app->renderer, 0, 100, 255, 80); 
            } else {
                SDL_SetRenderDrawColor(app->renderer, 255, 50, 50, 80);
            }

            for (int dr = -1; dr <= 1; dr++) {
                for (int dc = -1; dc <= 1; dc++) {
                    int r = town->row + dr;
                    int c = town->col + dc;

                    if (r >= 0 && r < MAP_ROWS && c >= 0 && c < MAP_COLS) {
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
    // Note: The old spawnCount loop has been removed to allow dynamic ownership colors.
}

void renderBridgeBottomOverlays(SDL_Renderer *renderer, AppState *app)
{
    for (int row = 0; row < MAP_ROWS; row++)
    {
        for (int col = 0; col < MAP_COLS; col++)
        {
            if (map_data[row][col] != 'b')
                continue;

            SDL_FRect destRect =
                camera2d_world_to_screen_rect(
                    &app->camera,
                    (float)(col * TILE_SIZE),
                    (float)(row * TILE_SIZE),
                    (float)TILE_SIZE,
                    (float)TILE_SIZE
                );

            SDL_RenderTexture(renderer,
                              app->bridgeBottomOverlayTexture,
                              NULL,
                              &destRect);
        }
    }
}

void renderTownOverlays(SDL_Renderer *renderer, AppState *app)
{
    for (int row = 0; row < MAP_ROWS; row++)
    {
        for (int col = 0; col < MAP_COLS; col++)
        {
            if (map_data[row][col] != 'C')
                continue;

            SDL_FRect destRect =
                camera2d_world_to_screen_rect(
                    &app->camera,
                    (float)(col * TILE_SIZE),
                    (float)(row * TILE_SIZE),
                    (float)TILE_SIZE,
                    (float)TILE_SIZE
                );

            SDL_RenderTexture(renderer,
                              app->townOverlayTexture,
                              NULL,
                              &destRect);
        }
    }
}