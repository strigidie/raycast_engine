#include "engine.h"

glrender_t* glrender;

void R_Init(uint32_t width, uint32_t height)
{
    glrender = (glrender_t*)malloc(sizeof(glrender_t));
    memset(glrender, 0, sizeof(glrender_t));
    glrender->width = width;
    glrender->height = height;
    glrender->context = SDL_GL_CreateContext(window);

    glrender->frame_buffer = (uint32_t*)malloc(sizeof(uint32_t) * (glrender->width * glrender->height));
    memset(glrender->frame_buffer, 0, sizeof(uint32_t) * (glrender->width * glrender->height));

    glrender->font = TTF_OpenFont("engine_font.ttf", 32);

    if (!glrender->context)
    {
        printf("[Error]: SDL_GL_CreateContext(), code: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, glrender->width, glrender->height, 0, -1, 1);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void R_Shutdown(void)
{
    SDL_GL_DeleteContext(glrender->context);
    free(glrender);
}

void R_DrawGUI(double delta_time)
{
    GUI_DrawCross();
    GUI_DrawFPS(delta_time);
}

void GUI_DrawCross(void)
{
    float crossWidth = glrender->width * 0.03;
    float crossHeight = glrender->height * 0.03;

    float crossX = (glrender->width - crossWidth) / 2.0;
    float crossY = (glrender->height - crossHeight) / 2.0;

    for (size_t x = crossX + crossWidth * 0.4; x < crossX + crossWidth * 0.6; x++)
        for (size_t y = crossY; y < crossY + crossHeight; y++)
            glrender->frame_buffer[x + y * glrender->width] = 0xffffffff;

    for (size_t x = crossX; x < crossX + crossWidth; x++)
        for (size_t y =  crossY + crossHeight * 0.4; y < crossY + crossHeight * 0.6; y++)
            glrender->frame_buffer[x + y * glrender->width] = 0xffffffff;
}

void GUI_DrawFPS(double delta_time)
{
    char txt_fps[64];
    sprintf(txt_fps, "FPS: %u",  (uint32_t)(1 / delta_time));

    SDL_Color font_fps_color = {255, 255, 255, 255};
    SDL_Surface* ctext = TTF_RenderText_Solid(glrender->font, txt_fps, font_fps_color);
    SDL_Surface* text = SDL_ConvertSurfaceFormat(ctext, SDL_PIXELFORMAT_RGBA8888, 0);

    for (size_t x = 0; x < (size_t)text->w; x++)
        for (size_t y = 0; y < (size_t)text->h; y++)
            glrender->frame_buffer[(x + 10) + (y + 10) * glrender->width] = ((uint32_t*)text->pixels)[x + y * text->w];

    SDL_FreeSurface(text);
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, glrender->width, glrender->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, glrender->frame_buffer);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, bufferTextureId);

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2f(0, 0);
    glTexCoord2f(1, 0); glVertex2f(glrender->width, 0);
    glTexCoord2f(1, 1); glVertex2f(glrender->width, glrender->height);
    glTexCoord2f(0, 1); glVertex2f(0, glrender->height);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glDeleteTextures(1, &bufferTextureId);
}