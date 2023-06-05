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
}

void R_Destroy(void)
{
    SDL_GL_DeleteContext(glcontext);
}   

void R_DrawFrame(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLuint bufferTextureId;
    glGenTextures(1, &bufferTextureId);
    glBindTexture(GL_TEXTURE_2D, bufferTextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, winfo->width, winfo->height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, frameBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, winfo->width, winfo->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, frameBuffer);

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