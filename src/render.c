#include "engine.h"

TTF_Font* engine_font;
SDL_Surface* text_font; 

SDL_Surface** textures;
SDL_Surface** sprites;

Mix_Chunk* sound;

uint32_t** map;

double* ZBuffer;

typedef struct Sprite_s
{
  double x;
  double y;
  int texture;
} Sprite;

#define numSprites 19

Sprite sprite[numSprites] =
{
  {20.5, 11.5, 1},
  {18.5,4.5, 1},
  {10.0,4.5, 1},
  {10.0,12.5,1},
  {3.5, 6.5, 1},
  {3.5, 20.5,1},
  {3.5, 14.5,1},
  {14.5,20.5,1},
  {18.5, 10.5, 2},
  {18.5, 11.5, 2},
  {18.5, 12.5, 2},
  {21.5, 1.5, 3},
  {15.5, 1.5, 3},
  {16.0, 1.8, 3},
  {16.2, 1.2, 3},
  {3.5,  2.5, 3},
  {9.5, 15.5, 3},
  {10.0, 15.1,3},
  {10.5, 15.8,3},
};

//arrays used to sort the sprites
int spriteOrder[numSprites];
double spriteDistance[numSprites];

//function used to sort the sprites
void sortSprites(int* order, double* dist, int amount);

void R_Init_Extra(void)
{
    FILE *texCountFile = fopen("assets/texture/info.txt", "r");
    
    if (!texCountFile)
    {
        printf("[Error]: Opening the file;\n");
        exit(EXIT_FAILURE);
    }
    
    int texCount;
    if (fscanf(texCountFile, "%d", &texCount) != 1)
    {
        printf("[Error]: Reading the number from the file;\n");
        fclose(texCountFile);
        exit(EXIT_FAILURE);
    }
    
    fclose(texCountFile);

    /* Check sprite count */
    FILE *sprCountFile = fopen("assets/sprite/info.txt", "r");
    
    if (!sprCountFile)
    {
        printf("[Error]: Opening the file;\n");
        exit(EXIT_FAILURE);
    }
    
    int sprCount;
    if (fscanf(sprCountFile, "%d", &sprCount) != 1)
    {
        printf("[Error]: Reading the number from the file;\n");
        fclose(sprCountFile);
        exit(EXIT_FAILURE);
    }
    
    fclose(sprCountFile);

    /* Load textures */
    char tmpFilePath[50];
    textures = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * texCount);
    for (size_t i = 0; i < texCount; ++i)
    {
        snprintf(tmpFilePath, 50, "assets/texture/%zu.png", i + 1);
        textures[i] = IMG_Load(tmpFilePath);

        if (!textures[i])
        {
            printf("[Error]: Can't load an image '%s';\n", tmpFilePath);
            exit(EXIT_FAILURE);
        }
    }

    /* Load sprites */
    sprites = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * sprCount);
    for (size_t i = 0; i < sprCount; ++i)
    {
        snprintf(tmpFilePath, 50, "assets/sprite/%zu.png", i + 1);
        sprites[i] = IMG_Load(tmpFilePath);

        if (!sprites[i])
        {
            printf("[Error]: Can't load an image '%s';\n", tmpFilePath);
            exit(EXIT_FAILURE);
        }
    }
    
    /* Load a font */
    engine_font = TTF_OpenFont("assets/font/font.ttf", winfo->height / 15);
    if (!engine_font)
    {
        printf("[Error]: Can't load font;\n");
        exit(EXIT_FAILURE);
    }

    /* Load a sound */
    int result = Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
    if (result != 0)
    {
        printf("[Error]: Can't load sound';\n");
        exit(EXIT_FAILURE);
    }

    sound = Mix_LoadWAV("assets/sound/sound.wav");
    Mix_PlayChannel(1, sound, -1);

    /* Load a map */
    FILE *mapFile = fopen("assets/map/map.txt", "r");
    
    if (!mapFile)
    {
        printf("[Error]: Opening the file;\n");
        exit(EXIT_FAILURE);
    }
    
    int width, height;
    if (fscanf(mapFile, "%d %d\n", &width, &height) != 2)
    {
        printf("[Error]: Reading the number from the file;\n");
        fclose(mapFile);
        exit(EXIT_FAILURE);
    }

    map = (uint32_t**)malloc(sizeof(uint32_t*) * height);
    for (int i = 0; i < height; i++)
    {
        map[i] = (uint32_t*)malloc(sizeof(uint32_t) * width);
        for (int j = 0; j < width; j++)
        {
            if (fscanf(mapFile, "%d", &map[i][j]) != 1)
            {
                printf("[Error]: Reading the numbers from the file;\n");
                fclose(mapFile);
                exit(EXIT_FAILURE);
            }
        }
    }
    
    fclose(mapFile);

    /* Create a frame buffer*/
    uint32_t amountBytes = sizeof(uint32_t) * (winfo->width * winfo->height);
    frameBuffer = (uint32_t*)malloc(amountBytes);
    if (!frameBuffer)
    {
        printf("[Error]: Allocate error for 'frameBuffer';\n");
        exit(EXIT_FAILURE);
    }
    memset(frameBuffer, 0, amountBytes);

    ZBuffer = (double*)malloc(sizeof(double) * winfo->width);
    memset(ZBuffer, 0, sizeof(double) * winfo->width);
}

#define texWidth 64
#define texHeight 64

double posX = 8.5, posY = 5;
double dirX = -1, dirY = 0;
double planeX = 0, planeY = 0.66;
double coffBright = 5.0f;

double fpsTimer = 0.0f;

void R_CameraUpdate(double deltaTime)
{
    const uint8_t* keys = SDL_GetKeyboardState(NULL);

    double moveSpeed = deltaTime * 3.0;
    double rotSpeed = deltaTime * 5.0;

    if (keys[SDL_SCANCODE_W])
    {
        if (map[(int)(posX + dirX * moveSpeed)][(int)posY] == false)
            posX += dirX * moveSpeed;
        if (map[(int)posX][(int)(posY + dirY * moveSpeed)] == false)
            posY += dirY * moveSpeed;
    }
    if (keys[SDL_SCANCODE_S])
    {
        if (map[(int)(posX - dirX * moveSpeed)][(int)posY] == false)
            posX -= dirX * moveSpeed;
        if (map[(int)posX][(int)(posY - dirY * moveSpeed)] == false)
            posY -= dirY * moveSpeed;
    }

    if (keys[SDL_SCANCODE_D])
    {
        double oldDirX = dirX;
        dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
        dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);

        double oldPlaneX = planeX;
        planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
        planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
    }
    if (keys[SDL_SCANCODE_A])
    {
        double oldDirX = dirX;
        dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
        dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
        double oldPlaneX = planeX;
        planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
        planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
    }
}

void R_Framebuffer(double deltaTime)
{
    int screenHeight = winfo->height;
    int screenWidth = winfo->width;

    for(int y = screenHeight / 2 + 1; y < screenHeight; ++y)
    {
        double rayDirX0 = dirX - planeX;
        double rayDirY0 = dirY - planeY;
        double rayDirX1 = dirX + planeX;
        double rayDirY1 = dirY + planeY;

        int p = y - screenHeight / 2;

        double posZ = 0.5 * screenHeight;

        double rowDistance = posZ / p;

        double floorStepX = rowDistance * (rayDirX1 - rayDirX0) / screenWidth;
        double floorStepY = rowDistance * (rayDirY1 - rayDirY0) / screenWidth;

        double floorX = posX + rowDistance * rayDirX0;
        double floorY = posY + rowDistance * rayDirY0;

        double brightness = 1.0f - (rowDistance / coffBright);
        if (brightness < 0)
            brightness = 0;
        for(int x = 0; x < screenWidth; ++x)
        {
            int cellX = (int)(floorX);
            int cellY = (int)(floorY);

            int tx = (int)(texWidth * (floorX - cellX)) & (texWidth - 1);
            int ty = (int)(texHeight * (floorY - cellY)) & (texHeight - 1);

            floorX += floorStepX;
            floorY += floorStepY;

            int checkerBoardPattern = ((int)(cellX + cellY)) & 1;
            int floorTexture;

            if(checkerBoardPattern == 0)
                floorTexture = 3;
            else
                floorTexture = 4;

            int ceilingTexture = 7;
            uint32_t color;

            color = ((uint32_t*)textures[floorTexture]->pixels)[texWidth * ty + tx];
            color = (color >> 1) & 8355711; 
            frameBuffer[x + y * winfo->width] = color;

            color = ((uint32_t*)textures[ceilingTexture]->pixels)[texWidth * ty + tx];
            color = (color >> 1) & 8355711;
            frameBuffer[(screenHeight - y - 1) * screenWidth + x] = color;
        }
    }

    for(int x = 0; x < screenWidth; x++)
    {
        double cameraX = 2 * x / (double)(screenWidth) - 1;
        double rayDirX = dirX + planeX * cameraX;
        double rayDirY = dirY + planeY * cameraX;

        int mapX = (int)(posX);
        int mapY = (int)(posY);

        double sideDistX;
        double sideDistY;

        double deltaDistX = (rayDirX == 0) ? 1e30 : fabs(1 / rayDirX);
        double deltaDistY = (rayDirY == 0) ? 1e30 : fabs(1 / rayDirY);
        double perpWallDist;

        int stepX;
        int stepY;

        int hit = 0;
        int side;

        if(rayDirX < 0)
        {
            stepX = -1;
            sideDistX = (posX - mapX) * deltaDistX;
        }
        else
        {
            stepX = 1;
            sideDistX = (mapX + 1.0 - posX) * deltaDistX;
        }
        if(rayDirY < 0)
        {
            stepY = -1;
            sideDistY = (posY - mapY) * deltaDistY;
        }
        else
        {
            stepY = 1;
            sideDistY = (mapY + 1.0 - posY) * deltaDistY;
        }

        while (hit == 0)
        {

            if(sideDistX < sideDistY)
            {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            }
            else
            {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }

            if(map[mapX][mapY] > 0)
                hit = 1;
        }

        if(side == 0)
            perpWallDist = (sideDistX - deltaDistX);
        else
            perpWallDist = (sideDistY - deltaDistY);

        int lineHeight = (int)(screenHeight / perpWallDist);

        int drawStart = -lineHeight / 2 + screenHeight / 2;
        if(drawStart < 0)
            drawStart = 0;

        int drawEnd = lineHeight / 2 + screenHeight / 2;
        if(drawEnd >= screenHeight)
            drawEnd = screenHeight - 1;

        int texNum = map[mapX][mapY] - 1;

        double wallX;
        if (side == 0)
            wallX = posY + perpWallDist * rayDirY;
        else
            wallX = posX + perpWallDist * rayDirX;
        wallX -= floor((wallX));

        int texX = (int)(wallX * (double)(texWidth));
        if(side == 0 && rayDirX > 0)
            texX = texWidth - texX - 1;
        if(side == 1 && rayDirY < 0)
            texX = texWidth - texX - 1;

        double step = 1.0 * texHeight / lineHeight;
        double texPos = (drawStart - screenHeight / 2 + lineHeight / 2) * step;
        for(int y = drawStart; y < drawEnd; y++)
        {
            int texY = (int)texPos & (texHeight - 1);
            texPos += step;
            uint32_t color = ((uint32_t*)textures[texNum]->pixels)[texHeight * texY + texX];
            if(side == 1)
                color = (color >> 1) & 8355711;
            frameBuffer[x + y * winfo->width] = color;
        }

        ZBuffer[x] = perpWallDist;
    }

    for(int i = 0; i < numSprites; i++)
    {
      spriteOrder[i] = i;
      spriteDistance[i] = ((posX - sprite[i].x) * (posX - sprite[i].x) + (posY - sprite[i].y) * (posY - sprite[i].y));
    }
    sortSprites(spriteOrder, spriteDistance, numSprites);

    for(int i = 0; i < numSprites; i++)
    {
      double spriteX = sprite[spriteOrder[i]].x - posX;
      double spriteY = sprite[spriteOrder[i]].y - posY;

      double invDet = 1.0 / (planeX * dirY - dirX * planeY);

      double transformX = invDet * (dirY * spriteX - dirX * spriteY);
      double transformY = invDet * (-planeY * spriteX + planeX * spriteY); 

      int spriteScreenX = (int)((screenWidth / 2) * (1 + transformX / transformY));

      #define uDiv 1
      #define vDiv 1
      #define vMove 0.0
      int vMoveScreen = (int)(vMove / transformY);

      int spriteHeight = abs((int)(screenHeight / (transformY))) / vDiv;
      int drawStartY = -spriteHeight / 2 + screenHeight / 2 + vMoveScreen;
      if(drawStartY < 0)
        drawStartY = 0;
      int drawEndY = spriteHeight / 2 + screenHeight / 2 + vMoveScreen;
      if(drawEndY >= screenHeight) drawEndY = screenHeight - 1;

      int spriteWidth = abs((int)(screenHeight / (transformY))) / uDiv;
      int drawStartX = -spriteWidth / 2 + spriteScreenX;
      if(drawStartX < 0) drawStartX = 0;
      int drawEndX = spriteWidth / 2 + spriteScreenX;
      if(drawEndX > screenWidth) drawEndX = screenWidth;

      for(int stripe = drawStartX; stripe < drawEndX; stripe++)
      {
        int texX = (int)(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * texWidth / spriteWidth) / 256;
        if(transformY > 0 && transformY < ZBuffer[stripe])
        {
          for(int y = drawStartY; y < drawEndY; y++)
          {
            int d = (y - vMoveScreen) * 256 - screenHeight * 128 + spriteHeight * 128; 
            int texY = ((d * texHeight) / spriteHeight) / 256;

            int spo = sprite[spriteOrder[i]].texture - 1;

            uint32_t color = ((uint32_t*)sprites[spo]->pixels)[texWidth * texY + texX];
            if((color & 0x00FFFFFF) != 0)
                frameBuffer[y * winfo->width + stripe] = color;
          }
        }
      }
    }


    static char str[50] = "0 FPS";
    if (fpsTimer < 1)
    {
        fpsTimer += deltaTime;
    }
    else
    {
        fpsTimer = 0.0f;
        snprintf(str, 50, "%d FPS", (int)(1.0f / deltaTime));
    }

    SDL_Color color = {255, 255, 255, 255};
    text_font = TTF_RenderText_Solid(engine_font, str, color);
    SDL_Surface* convertedSurface = SDL_ConvertSurfaceFormat(text_font, SDL_PIXELFORMAT_RGBA8888, 0);

    uint32_t* pixels = (uint32_t*)convertedSurface->pixels;
    for (size_t x = 0; x < convertedSurface->w; x++)
    {
        for (size_t y = 0; y < convertedSurface->h; y++)
        {
            uint32_t pixel = pixels[y * convertedSurface->w + x];
            if (pixel > 0)
            {
                frameBuffer[y * winfo->width + x] = pixel;
            }
        }
    }
}


void sortSprites(int* order, double* dist, int amount) {
    for (int i = 0; i < amount - 1; i++) {
        for (int j = 0; j < amount - i - 1; j++) {
            if (dist[j] < dist[j + 1]) {
                // Swap distance values
                double tempDist = dist[j];
                dist[j] = dist[j + 1];
                dist[j + 1] = tempDist;

                // Swap order values
                int tempOrder = order[j];
                order[j] = order[j + 1];
                order[j + 1] = tempOrder;
            }
        }
    }
}