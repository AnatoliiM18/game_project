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

/* Globals */
static SDL_Window* g_window;
static SDL_Renderer* g_renderer;
static int g_wnd_width;
static int g_wnd_height;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    /* Set startup window size */
    g_wnd_width = 640;
    g_wnd_height = 480;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        LOG_ERROR(SDL_Init);
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer(WND_TITLE, g_wnd_width, g_wnd_height,
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

    if (event->type == SDL_EVENT_WINDOW_RESIZED) {
        SDL_GetWindowSize(g_window, &g_wnd_width, &g_wnd_height);
#if 0 /* For little check */
        SDL_Log("WS\t%d\t%d", g_wnd_width, g_wnd_height);
        SDL_GetWindowSizeInPixels(g_window, &g_wnd_width, &g_wnd_height);
        SDL_Log("IP\t%d\t%d", g_wnd_width, g_wnd_height);
        SDL_GetRenderOutputSize(g_renderer, &g_wnd_width, &g_wnd_height);
        SDL_Log("RS\t%d\t%d", g_wnd_width, g_wnd_height);
#endif
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
    SDL_FRect rect;

    /* Clear the current rendering target at the start of every frame */
    SDL_SetRenderDrawColor(g_renderer, 0x00, 0x80, 0x80, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(g_renderer);

    /* Set the rect's size to half of the screen */
    rect.w = g_wnd_width / 2.0f;
    rect.h = g_wnd_height / 2.0f;
    /* Set the rect's position to the middle of the screen */
    rect.x = (g_wnd_width - rect.w) / 2;
    rect.y = (g_wnd_height - rect.h) / 2;

    /* Draw rect */
    SDL_SetRenderDrawColor(g_renderer, 0x80, 0x00, 0x00, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(g_renderer, &rect);

    /* Draw grid */
    SDL_SetRenderDrawColor(g_renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
    float grid_step = 20.0f;
    for (float y_pos = 0.0f; y_pos < g_wnd_height; y_pos += grid_step) {
        SDL_RenderLine(g_renderer, 0.0f, y_pos, g_wnd_width, y_pos);
    }
    for (float x_pos = 0.0f; x_pos < g_wnd_width; x_pos += grid_step) {
        SDL_RenderLine(g_renderer, x_pos, 0.0f, x_pos, g_wnd_height);
    }

    SDL_RenderPresent(g_renderer);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
}
