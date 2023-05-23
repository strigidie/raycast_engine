// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <SDL.h>

#include "map.h"

#define MAP_MENU_LOOP 1
#define BUFFER_SIZE 64

FILE* file = NULL;
void map_create(const char* fn)
{
    file = fopen(fn, "w");

    if (!file)
    {
        printf("error: cannot create a file;");
        exit(1);
    }
}

map_t* map = NULL;
void map_init(uint32_t number_levels, uint32_t width, uint32_t height)
{
    map = (map_t*)malloc(sizeof(map_t));
    if (!map)
    {
        printf("error: cannot allocate memory;");
        exit(1);
    }

    map->number_levels = number_levels;
    map->width = width;
    map->height = height;

    map->levels = (mapLevel_t*)malloc(sizeof(mapLevel_t) * map->number_levels);
    if (!map->levels)
    {
        printf("error: cannot allocate memory;");
        exit(1);
    }

    for (size_t id = 0; id < map->number_levels; id++)
    {
        map->levels[id].cells = (mapCell_t**)malloc(sizeof(mapCell_t*) * map->height);
        if (!map->levels[id].cells)
        {
            printf("error: cannot allocate memory;");
            exit(1);
        }

        for (size_t y = 0; y < map->height; y++)
        {
            map->levels[id].cells[y] = (mapCell_t*)malloc(sizeof(mapCell_t) * map->width);
            if (!map->levels[id].cells[y])
            {
                printf("error: cannot allocate memory;");
                exit(1);
            }

            for (size_t x = 0; x < map->width; x++)
            {
                map->levels[id].cells[y][x].entity = EMPTY;
                map->levels[id].cells[y][x].texture_face = 0x0;
            }
        }
    }
}

void map_print(void)
{
    if (!map)
    {
        printf("error: map doesn't exist.\n");
        return ;
    }

    printf("$numLevel = %u\n$width = %u\n$height = %u\n\n", map->number_levels, map->width, map->height);

    for (size_t id = 0; id < map->number_levels; ++id)
    {
        printf("id: %zu\n", id);
        for (size_t y = 0; y < map->width; y++)
        {
            printf("[");
            for (size_t x = 0; x < map->width; x++)
            {
                printf("x = %zu y = %zu ", x, y);
                switch (map->levels[id].cells[y][x].entity)
                {
                    case EMPTY:
                        printf("%5s ", "EMPTY");
                        break;
                    
                    case WALL:
                        printf("%5s ", "WALL");
                        break;
                    
                    case FLOOR:
                        printf("%5s ", "FLOOR");
                        break;
                    
                    case SKY:
                        printf("%5s ", "SKY");
                        break;
                    
                    case DOOR:
                        printf("%5s ", "DOOR");
                        break;
                    
                    case BUTTON:
                        printf("%5s ", "BUTTON");
                        break;
                    
                    case TELEPORT:
                        printf("%5s ", "TELEPORT");
                        break;
                }
            }
            printf("]\n");
        }
        printf("\n");
    }
}

void map_save(void)
{
    fwrite(&map->width, sizeof(uint32_t), 1, file);
    fwrite(&map->height, sizeof(uint32_t), 1, file);
    fwrite(&map->number_levels, sizeof(uint32_t), 1, file);

    for (uint32_t id = 0; id < map->number_levels; id++)
        for (uint32_t y = 0; y < map->height; y++)
            fwrite(map->levels[id].cells[y], sizeof(mapCell_t), map->width, file);
    
    fclose(file);
}

int32_t check_cmd(const char* cmd, const char* fn)
{
    if (strncmp(cmd, fn, BUFFER_SIZE) == 0)
        return 1;
    return 0;
}

int32_t map_menu(void)
{
    char cmd[BUFFER_SIZE];
    char fn[BUFFER_SIZE];

    while (MAP_MENU_LOOP)
    {
        memset(cmd, 0, 64);

        printf("> ");
        scanf("%s", cmd);

        if (check_cmd(cmd, "quit"))
        {
            break;
        }
        else if (check_cmd(cmd, "init"))
        {
            uint32_t number_levels = 0;
            uint32_t width = 0;
            uint32_t height = 0;

            printf("$name = ");
            scanf("%s", fn);

            map_create(fn);

            printf("$number_levels = ");
            scanf("%u", &number_levels);

            printf("$width = ");
            scanf("%u", &width);

            printf("$height = ");
            scanf("%u", &height);
            
            map_init(number_levels, width, height);
        }
        else if (check_cmd(cmd, "set"))
        {
            uint32_t id;
            printf("$id = ");
            scanf("%u", &id);

            uint32_t x;
            printf("$x = ");
            scanf("%u", &x);

            uint32_t y;
            printf("$y = ");
            scanf("%u", &y);

            mapEntity_t entity;
            printf("mapEntity_t:\n - EMPTY = 0\n - WALL = 1\n - FLOOR = 2\n - SKY = 3\n - DOOR = 4\n - BUTTON = 5\n - TELEPORT\n");
            printf("$entity = ");
            scanf("%d", &entity);

            map->levels[id].cells[y][x].entity = entity;

            // mapFaceTexture_t texture_face;

            // mapTextureCode_t right;
            // printf("$right = ");
            // scanf("%u", &right);

            // mapTextureCode_t left;
            // mapTextureCode_t top;
            // mapTextureCode_t down;
            // mapTextureCode_t front;
            // mapTextureCode_t back;
        }
        else if (check_cmd(cmd, "print"))
        {
            map_print();
        }
        else if (check_cmd(cmd, "save"))
        {
            map_save();
        }
    }

    return 0;
}

int32_t main(void)
{   
    return map_menu();
}