#include "engine.h"

SDL_Window* window = NULL;

void W_Init(uint32_t width, uint32_t height)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("[Error]: SDL_Init(), code: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // For Fullscreen
    // SDL_SetWindowGrab(window, true);
    // SDL_SetRelativeMouseMode(true);

    window = SDL_CreateWindow("Raycast",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_OPENGL
    );

    if (!window)
    {
        printf("[Error]: SDL_CreateWindow(), code: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    if (TTF_Init() == -1)
    {
        printf("[Error]: TTF_Init()\n");
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
        R_DrawFrame();
        R_DrawGUI(deltaTime);
        SDL_GL_SwapWindow(window);
    }
}

void W_Destroy(void)
{
    SDL_DestroyWindow(window);
    SDL_Quit();
}