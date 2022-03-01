#include "window.h"
#include "general.h"

int main(int argc, char* argv[]) {
    Window window;
    while (window.isOpen) {
        window.input();
        window.update();
        window.draw();
        SDL_Delay(1000 / 60);
    }
    return 0;
}