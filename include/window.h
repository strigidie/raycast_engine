#ifndef WINDOW_H
#define WINDOW_H

#define DEFUALT_WINDOW_WIDTH 800
#define DEFUALT_WINDOW_HEIGHT 600

extern SDL_Window* window;

void W_Init(uint32_t width, uint32_t height);
void W_MainLoop(void);
void W_Destroy(void);

#endif