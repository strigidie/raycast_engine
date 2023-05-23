#ifndef RENDER_H
#define RENDER_H

// extern uint32_t **frame_buffer;
// extern SDL_GLContext glcontext;

typedef struct glrender_s
{
    uint32_t width;
    uint32_t height;
    uint32_t *frame_buffer;
    SDL_GLContext context;
    TTF_Font* font;
} glrender_t;

extern glrender_t* glrender;

void R_Init(uint32_t width, uint32_t height);
void R_Shutdown(void);

void R_DrawFrame(void);

// Render GUI
void R_DrawGUI(double delta_time);
void GUI_DrawFPS(double delta_time);
void GUI_DrawCross(void);

#endif