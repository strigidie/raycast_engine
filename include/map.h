#ifndef MAP_H
#define MAP_H

// -----------------------------------------------------------------------------
// Macros
// -----------------------------------------------------------------------------
#define DEFAULT_MAP "default.map"
#define DEFAULT_MAP_WIDTH 10
#define DEFAULT_MAP_HEIGHT 10
#define DEFAULT_MAP_NUMBER_LEVElS 2

// -----------------------------------------------------------------------------
// Custom Data Types
// -----------------------------------------------------------------------------
/* Map texture manager */
typedef uint8_t mapTextureCode_t;
typedef uint64_t mapFaceTexture_t;

typedef struct mapTextureManager_s
{
    mapTextureCode_t code;
    uint32_t color_face;
    SDL_Surface* texture;
    struct mapTexture_s* next;
} mapTextureManager_t;

/* Map organization */
typedef enum mapEntity_e
{
    EMPTY = 0,
    WALL,
    FLOOR,
    SKY,
    DOOR,
    BUTTON,
    TELEPORT
} mapEntity_t;

typedef struct mapCell_s
{
    mapEntity_t entity;
    mapFaceTexture_t texture_face;
} mapCell_t;

typedef struct mapLevel_s
{
    mapCell_t** cells;
} mapLevel_t;

typedef struct map_s
{
    uint32_t width;
    uint32_t height;
    uint32_t number_levels;
    mapLevel_t* levels;
} map_t;

// -----------------------------------------------------------------------------
// Extern Variables
// -----------------------------------------------------------------------------
extern map_t* map;
extern mapTextureManager_t* map_texture_manager;

// -----------------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------------
void M_Init(const char *fn);
void M_Destroy(void);

#endif