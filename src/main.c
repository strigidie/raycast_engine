#include "engine.h"

winfo_t* winfo;

int main(int argc, char* argv[])
{
    winfo = (winfo_t*)malloc(sizeof(winfo_t));
    if (!winfo)
    {
        printf("[Error]: Allocate error for 'winfo';\n");
        exit(EXIT_FAILURE);
    }

    winfo->width = DEFUALT_WINDOW_WIDTH;
    winfo->height = DEFUALT_WINDOW_HEIGHT;
    winfo->fullscreen = false;

    W_Init();
    R_Init();
    // F_Init();
    // M_Init();

    W_MainLoop();

    // R_Shutdown();
    W_Destroy();
    free(winfo);

    return EXIT_SUCCESS;
}
