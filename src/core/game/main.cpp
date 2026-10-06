#include "game.h"
#include <SDL3/SDL_main.h>
int main(int, char*[]) {
    Game game;
    while (game.isOpen) game.loop();
    return 0;
}