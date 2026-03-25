#include "../game.h"
#include "map.h"
#include <stdio.h>


char map_data[MAP_ROWS][MAP_COLS];


// Xavier with Cesar's help
void loadMap(const char *filename)
{
    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        printf("Failed to open map file");
        return;
    }

    printf("loading map %s", filename);
    for (int row = 0; row < MAP_ROWS; row++)
    {
        for (int col = 0; col < MAP_COLS; col++)
        {
            int ch = fgetc(file);
            if (ch == EOF)
            {
                fclose(file);
                return;
            }
            if (ch == '\n')
            {
                col--;
                continue;
            }
            map_data[row][col] = (char)ch;
        }
    }
    fclose(file);
}

//made with Gemini (Xavier), ask Cesar if it ok
void renderMap(SDL_Renderer *renderer, AppState *app) {
    for (int row = 0; row < MAP_ROWS; row++) {
        for (int col = 0; col < MAP_COLS; col++) {
            SDL_FRect destRect = {
                (float)(col * TILE_SIZE), 
                (float)(row * TILE_SIZE), 
                (float)TILE_SIZE, 
                (float)TILE_SIZE
            };

            SDL_Texture *targetTexture = NULL;

            // Map the character in the txt file to the loaded texture
            switch (map_data[row][col]) {
                case 'G': targetTexture = app->grassTexture; break;
                case 'W': targetTexture = app->waterTexture; break;
                case 'B': targetTexture = app->bridgeTexture; break;
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
