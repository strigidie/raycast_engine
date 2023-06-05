#include "engine.h"

SDL_Renderer* render; 
SDL_Surface* sdl_frameBuffer;
SDL_Texture* texture;

void R_Init(void)
{
    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    sdl_frameBuffer = SDL_CreateRGBSurface(0, winfo->width, winfo->height, 32, 0, 0, 0, 0);
}

void R_Destroy(void)
{

}   

void R_DrawFrame(void)
{
    Uint32 pixelFormat = SDL_PIXELFORMAT_RGBA32;
    int pitch = winfo->width * sizeof(Uint32);

    SDL_LockSurface(sdl_frameBuffer); 
    SDL_ConvertPixels(winfo->width, winfo->height, pixelFormat, frameBuffer, pitch,
                    sdl_frameBuffer->format->format, sdl_frameBuffer->pixels, sdl_frameBuffer->pitch);
    SDL_UnlockSurface(sdl_frameBuffer);

    texture = SDL_CreateTextureFromSurface(render, sdl_frameBuffer);
    SDL_RenderClear(render);
    SDL_RenderCopy(render, texture, NULL, NULL);
    SDL_RenderPresent(render);
}