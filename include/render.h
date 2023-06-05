#ifndef RENDER_H
#define RENDER_H

extern uint32_t* frameBuffer;

void R_Init(void);
void R_Destroy(void);
void R_DrawFrame(void);

void R_Init_Extra(void);
void R_Framebuffer(double deltaTime);
void R_CameraUpdate(double deltaTime);

#endif