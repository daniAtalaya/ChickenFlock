
#include "window.h"
#include "general.h"

int main(int argc, char* argv[]) {
    Window window("Daniel Atalaya's Second SDL2 Window");
    while (!window.isClosed()) {
        window.update();
        window.draw();
        SDL_Delay(1000 / 60);
    }
    return 0;
}