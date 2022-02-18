#include <iostream>
#include <SDL.h>
int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cout << "Failed to initialize the SDL2 library" << std::endl << "SDL2 Error: " << SDL_GetError() << std::endl;
        return -1;
    }
    SDL_Window* window = SDL_CreateWindow("Daniel Atalaya's First SDL2 Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 680, 480, 0);
    if (!window) {
        std::cout << "Failed to create window" << std::endl << "SDL2 Error: " << SDL_GetError() << std::endl;
        return -1;
    }
    SDL_Surface* window_surface = SDL_GetWindowSurface(window);
    if (!window_surface) {
        std::cout << "Failed to get the surface from the window" << std::endl << "SDL2 Error: " << SDL_GetError() << std::endl;
        return -1;
    }
    SDL_Surface* image = SDL_LoadBMP("guerrilla-war.bmp");
    if (!image) {
        std::cout << "Failed to load image" << std::endl << "SDL2 Error: " << SDL_GetError() << std::endl;
        return -1;
    }
    while (true) {
        SDL_Event e;
        while (SDL_PollEvent(&e) > 0) {
            switch (e.type) {
                case SDL_QUIT:
                    return 0;
            }
            SDL_BlitSurface(image, NULL, window_surface, NULL);
            SDL_UpdateWindowSurface(window);
        }
    }
    return 0;
}