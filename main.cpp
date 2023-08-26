#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <getopt.h>
#include <iostream>

#include <SDL2/SDL.h>  

bool gameRender();
bool gameLogic();

int main(int argc, char *argv[])
{
    srand(time(nullptr));
    int fullScreen = false, vSync = false, unlocked = true, noSplash = true, customResolution = false, performance = false;
    int frameCap = 30, c, width = 1280, height = 960;

    while(true) {
        const char *const short_opts = "w:h:c:fvunp";
        const struct option long_opts[] = {
                {"frameCap",   required_argument, nullptr,     'c'},
                {"fullScreen", no_argument,       &fullScreen, '\0'},
                {"performance",no_argument,       &performance, '\0'},
                {"vSync",      no_argument,       &vSync,      '\0'},
                {"unlocked",   no_argument,       &unlocked,   'u'},
                {"noSplash",   no_argument,       &noSplash,   'n'},
                {"width",      required_argument, nullptr,     'w'},
                {"height",     required_argument, nullptr,     'h'},
        };

        int option_index = 0;

        c = getopt_long(argc, argv, short_opts, long_opts, &option_index);

        if (c == -1)
            break;

        for (int i = 0; i < argc; i++) {
            switch (c) {
                case 0:
                    if (long_opts[option_index].flag != nullptr)
                        *long_opts[option_index].flag = true;
                    break;
                case 'c':
                    frameCap = (int)strtol(optarg, nullptr, 10);
                    std::cout << "Capping the framerate is not yet supported, and as such the -c option is ignored." << std::endl;
                    continue;
                case 'w':
                    width = (int)strtol(optarg, nullptr, 10);
                    customResolution = true;
                    continue;
                case 'h':
                    height = (int)strtol(optarg, nullptr, 10);
                    customResolution = true;
                    continue;
                case 'f':
                    fullScreen = true;
                    continue;
                case 'v':
                    vSync = true;
//                    std::cout << "Toggling VSync is not yet supported, and as such the -v flag is ignored." << std::endl;
                    continue;
                case 'u':
                    unlocked = true;
                    std::cout << "Capping the framerate is not yet supported, and as such the -u flag is ignored." << std::endl;
                    continue;
                case 'n':
                    noSplash = true;
                    continue;
                default:
                    break;
            }
        }
    }

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow(
        "Minesweeper",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width,
        height,
        0
    );

    int rendering_flags = SDL_RENDERER_ACCELERATED;

    if(!vSync) {
        rendering_flags |= SDL_RENDERER_PRESENTVSYNC;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, rendering_flags);

    // deltaTime variables
    // -------------------
    double deltaTime = 0.0f;
    double lastFrame = 0.0f;
    double frameTime = 0;

    bool gameIsRunning = true;

    while(gameIsRunning) {
        try {
            
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderClear(renderer);

            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                switch(event.type) {
                    case(SDL_QUIT):
                    gameIsRunning = false;
                    break;
                }
            }

            if(!gameRender()) {
                gameIsRunning = false;
            }

            if(!gameLogic()) {
                gameIsRunning = false;
            }

            SDL_RenderPresent(renderer);

        } catch(std::exception& e){
            std::cout << e.what() << std::endl;
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

bool gameRender() {
    return true;
}

bool gameLogic() {
    return true;
}