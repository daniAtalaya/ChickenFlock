#include "game.h"
int main(int argc, char* argv[]) {
    Game game;
    while (game.isOpen) game.loop();
    return 0;
}