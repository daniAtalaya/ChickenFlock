#include <iostream>
#include "window.h"
#include "SDL/include/SDL.h"
#pragma comment( lib, "SDL/x64/SDL2.lib" )
#pragma comment( lib, "SDL/x64/SDL2main.lib" )

int main(int argc, char* argv[]) {
    Window window("Daniel Atalaya's Second SDL2 Window");
    while (!window.isClosed()) {
        window.update();
        window.draw();
        SDL_Delay(1000 / 60);
    }
    return 0;
}