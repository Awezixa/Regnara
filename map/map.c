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