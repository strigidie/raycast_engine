#ifndef RENDER_H
#define RENDER_H

extern uint32_t* frameBuffer;

void R_Init(void);
void R_Shutdown(void);
void R_DrawFrame(void);

#endif