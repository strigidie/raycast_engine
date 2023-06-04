#include "engine.h"

SDL_GLContext glcontext;
uint32_t* frameBuffer;

void R_Init(void)
{
    glcontext = SDL_GL_CreateContext(window);
    if (!glcontext)
    {
        printf("[Error]: SDL_GL_CreateContext();\n");
        exit(EXIT_FAILURE);
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, winfo->width, winfo->height, 0, -1, 1);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    uint32_t amountBytes = sizeof(uint32_t) * (winfo->width * winfo->height);
    frameBuffer = (uint32_t*)malloc(amountBytes);
    if (!frameBuffer)
    {
        printf("[Error]: Allocate error for 'frameBuffer';\n");
        exit(EXIT_FAILURE);
    }
    memset(frameBuffer, 0, amountBytes);
}

void R_Shutdown(void)
{
    SDL_GL_DeleteContext(glcontext);
}   

void R_DrawFrame(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

     int stripWidth = winfo->width / 5;
    int stripHeight = winfo->height;

    // Manually specify pixel format
    SDL_PixelFormat format;
    format.format = SDL_PIXELFORMAT_RGBA8888;
    format.BitsPerPixel = 32;
    format.BytesPerPixel = 4;
    format.Rmask = 0x000000FF;
    format.Gmask = 0x0000FF00;
    format.Bmask = 0x00FF0000;
    format.Amask = 0xFF000000;

    // Draw the strips and write color values to frameBuffer
    for (int x = 0; x < winfo->width; x++)
    {
        int stripIndex = x / stripWidth;
        uint32_t color;

        // Determine color based on strip index
        switch (stripIndex)
        {
            case 0:
                color = 0xFF716A4E; // RGB color: (113, 106, 78)
                break;
            case 1:
                color = 0xFF4E3B35; // RGB color: (78, 59, 53)
                break;
            case 2:
                color = 0xFF6C4B36; // RGB color: (108, 75, 54)
                break;
            case 3:
                color = 0xFFB45E33; // RGB color: (180, 94, 51)
                break;
            case 4:
                color = 0xFF1C3B33; // RGB color: (28, 59, 51)
                break;
            default:
                color = 0xFF000000; // Default to black
                break;
        }

        for (int y = 0; y < stripHeight; y++)
        {
            int index = y * winfo->width + x;
            frameBuffer[index] = color;
        }
    }

    GLuint bufferTextureId;
    glGenTextures(1, &bufferTextureId);
    glBindTexture(GL_TEXTURE_2D, bufferTextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, winfo->width, winfo->height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, frameBuffer);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, bufferTextureId);

    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2i(0, 0);
        glTexCoord2f(1, 0); glVertex2i(winfo->width, 0);
        glTexCoord2f(1, 1); glVertex2i(winfo->width, winfo->height);
        glTexCoord2f(0, 1); glVertex2i(0, winfo->height);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glDeleteTextures(1, &bufferTextureId);
}