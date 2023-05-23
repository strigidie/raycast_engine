#include "engine.h"

int main(void)
{
    // M_Init(DEFAULT_MAP);

    W_Init(DEFUALT_WINDOW_WIDTH, DEFUALT_WINDOW_HEIGHT);
    R_Init(DEFUALT_WINDOW_WIDTH, DEFUALT_WINDOW_HEIGHT);

    W_MainLoop();

    R_Shutdown();
    W_Destroy();
    M_Destroy();

    return EXIT_SUCCESS;
}