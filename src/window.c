#include "engine.h"

winfo_t* winfo;

SDL_Window* window = NULL;

void W_Init(void)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("[Error]: SDL_Init(), code: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    uint32_t sdl_flags = 0;
    if (winfo->fullscreen)
        sdl_flags |= SDL_WINDOW_FULLSCREEN;

    switch (winfo->API)
    {
        default:
        case API_OPENGL:
            sdl_flags |= SDL_WINDOW_OPENGL;
            break;
        
        case API_VULKAN:
            sdl_flags |= SDL_WINDOW_VULKAN;
            break;
    }

    window = SDL_CreateWindow(ENGINE_TITLE,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        winfo->width, winfo->height,
        sdl_flags
    );

    if (!window)
    {
        printf("[Error]: SDL_CreateWindow(), code: %s;\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    if (winfo->fullscreen)
    {
        SDL_SetWindowGrab(window, true);
        SDL_SetRelativeMouseMode(true);
    }


    if (TTF_Init() == -1)
    {
        printf("[Error]: TTF_Init();\n");
        exit(EXIT_FAILURE);
    }
}

void W_MainLoop()
{
    SDL_Event event;

    bool quit = false;
    double deltaTime = 0;
    double currectTick = 0;
    double previousTick = 0;
    while (MAIN_ENGINE_LOOP)
    {
        if (quit)
            break;
        
        currectTick = SDL_GetTicks();
        deltaTime = (currectTick - previousTick) / 1000.0f;
        previousTick = currectTick;
        if (deltaTime > 60 / 1000.0f)
            deltaTime = 60 / 1000.0f;

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    quit = true;
                
                case SDL_KEYDOWN:
                case SDL_KEYUP:
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                        {
                            quit = true;
                            break;
                        }
                    }
            }
        }
    
        // R_DrawMap();
        // R_DrawFrame();
        // R_DrawGUI(deltaTime);
        SDL_GL_SwapWindow(window);
    }
}

void W_Destroy(void)
{
    SDL_DestroyWindow(window);
    SDL_Quit();
}