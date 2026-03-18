#include "map.h"
#include "camera/camera.h"



// Xavier with Cesar's help
// void loadMap(const char *filename)
// {
//     // char extension[] = ".txt";
//     // char levelTens[3];
//     // sprintf(levelTens, "%d", levelSelect);
//     // char filename[32];
//     // sprintf(filename, "Maps/map%s%s", levelTens, extension);

//     FILE *file = fopen(filename, "r");

//     if (file == NULL)
//     {
//         printf("Failed to open map file");
//         return;
//     }

//     printf("loading map %s", filename);
//     for (int row = 0; row < MAP_ROWS; row++)
//     {
//         for (int col = 0; col < MAP_COLS; col++)
//         {
//             int ch = fgetc(file);
//             if (ch == EOF)
//             {
//                 fclose(file);
//                 return;
//             }
//             if (ch == '\n')
//             {
//                 col--;
//                 continue;
//             }
//             map[row][col] = (char)ch;
//         }
//     }
//     fclose(file);
// }