#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf\SDL_ttf.h>

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
SDL_Texture* g_text_texture;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    /* Set startup window size */
    g_wnd_width = 640;
    g_wnd_height = 480;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        LOG_ERROR(SDL_Init);
        return SDL_APP_FAILURE;
    }

    if (!TTF_Init()) {
        LOG_ERROR(TTF_Init);
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer(WND_TITLE, g_wnd_width, g_wnd_height,
        SDL_WINDOW_RESIZABLE, &g_window, &g_renderer)) {

        LOG_ERROR(SDL_CreateWindowAndRenderer);
        return SDL_APP_FAILURE;
    }

    /* Work with text */
    TTF_Font* font = TTF_OpenFont("../assets/Xolonium-Bold.ttf", 36.0f);
    if (!font) {
        LOG_ERROR(TTF_OpenFont);
        return SDL_APP_FAILURE;
    }

    const char* text = "Test string.*+- 1 2 ! 5 90 O % / \\ 0 6 j i";
    SDL_Color text_color = { 0xFF, 0x80, 0x80, SDL_ALPHA_OPAQUE };
    SDL_Surface* text_surf = TTF_RenderText_Solid(font, text, 0, text_color);
    g_text_texture = SDL_CreateTextureFromSurface(g_renderer, text_surf);

    TTF_CloseFont(font);
    SDL_DestroySurface(text_surf);

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
    SDL_SetRenderDrawColor(g_renderer, 0x80, 0x80, 0x80, SDL_ALPHA_OPAQUE);
    float grid_step = 10.0f;
    for (float y_pos = 0.0f; y_pos < g_wnd_height; y_pos += grid_step) {
        SDL_RenderLine(g_renderer, 0.0f, y_pos, g_wnd_width, y_pos);
    }
    for (float x_pos = 0.0f; x_pos < g_wnd_width; x_pos += grid_step) {
        SDL_RenderLine(g_renderer, x_pos, 0.0f, x_pos, g_wnd_height);
    }

    /* Draw text */
    SDL_FRect dst_text_rect;
    dst_text_rect.x = rect.x;
    dst_text_rect.y = rect.y;
    dst_text_rect.w = g_text_texture->w;
    dst_text_rect.h = g_text_texture->h;
    SDL_RenderTexture(g_renderer, g_text_texture, NULL, &dst_text_rect);

    SDL_RenderPresent(g_renderer);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    TTF_Quit();
}
