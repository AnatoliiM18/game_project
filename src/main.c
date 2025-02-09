#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

/* Extended SDL_Log() macro (includes file name and line number) */
#define LOG_ERROR(func_name)    do {                                    \
                                    SDL_Log("[%s:%d] %s error: %s",     \
                                            __FILE__, __LINE__,         \
                                            #func_name, SDL_GetError());\
                                } while (0)

/* Global defines */
#define WND_TITLE   "Window title"
#define WND_WIDHT   640
#define WND_HEIGHT  480

/* Globals */
static SDL_Window* g_window;
static SDL_Renderer* g_renderer;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        LOG_ERROR(SDL_Init);
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer(WND_TITLE, WND_WIDHT, WND_HEIGHT,
        SDL_WINDOW_RESIZABLE, &g_window, &g_renderer)) {

        LOG_ERROR(SDL_CreateWindowAndRenderer);
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
    SDL_SetRenderDrawColor(g_renderer, 0x00, 0xFF, 0x00, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(g_renderer);
    SDL_RenderPresent(g_renderer);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
}
