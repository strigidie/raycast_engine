#include "engine.h"

winfo_t* winfo;

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
#elif __APPLE__
int main(int argc, const char* argv[])
#endif
{
    winfo = (winfo_t*)malloc(sizeof(winfo_t));
    if (!winfo)
    {
        printf("[Error]: Allocate Error;\n");
        exit(EXIT_FAILURE);
    }

    winfo->width = DEFUALT_WINDOW_WIDTH;
    winfo->height = DEFUALT_WINDOW_HEIGHT;
    winfo->fullscreen = false;
    winfo->API = API_OPENGL;

    W_Init();
    // R_Init();
    // F_Init();
    // M_Init();

    W_MainLoop();

    // R_Shutdown();
    W_Destroy();

    return EXIT_SUCCESS;
}
