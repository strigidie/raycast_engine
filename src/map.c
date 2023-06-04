#include "engine.h"

// map_t* map;
// mapTextureManager_t* mapTextureManager;

// void M_Init(const char *fn)
// {
//     FILE* file = fopen(fn, "rb");

//     if (file == NULL)
//     {
//         printf("error: cannot open file '%s'\n", fn);
//         exit(1);
//         return ;
//     }

//     map_t* map = (map_t*)malloc(sizeof(map_t));
//     fread(&map->width, sizeof(map->width), 1, file);
//     fread(&map->height, sizeof(map->height), 1, file);
//     fread(&map->number_levels, sizeof(map->number_levels), 1, file);

//     map->levels = (mapLevel_t*)malloc(sizeof(mapLevel_t) * map->number_levels);

//     for (uint32_t id = 0; id < map->number_levels; id++)
//     {
//         map->levels[id].cells = (mapCell_t**)malloc(sizeof(mapCell_t*) * map->height);
//         for (uint32_t y = 0; y < map->height; y++)
//         {
//             map->levels[id].cells[y] = (mapCell_t*)malloc(sizeof(mapCell_t) * map->width);
//             fread(map->levels[id].cells[y], sizeof(mapCell_t), map->width, file);
//         }
//     }

//     fclose(file);
// }

// void M_Destroy(void)
// {    
//     // for (uint32_t id = 0; id < map->number_levels; id++)
//     // {
//     //     for (uint32_t y = 0; y < map->height; y++)
//     //         free(map->levels[id].cells[y]);
//     //     free(map->levels[id].cells);
//     // }

//     // free(map->levels);
//     // free(map);
// }