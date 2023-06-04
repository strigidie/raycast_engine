#ifndef WINDOW_H
#define WINDOW_H

#define ENGINE_TITLE "Raycast Engine"

#define DEFUALT_WINDOW_WIDTH 800
#define DEFUALT_WINDOW_HEIGHT 600

typedef struct winfo_s
{
    uint32_t width;
    uint32_t height;
    bool fullscreen;
} winfo_t;

extern winfo_t* winfo;
extern SDL_Window* window;

void W_Init(void);
void W_MainLoop(void);
void W_Destroy(void);

#endif