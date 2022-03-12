#include "game.h"
#include "general.h"

bool Game::god = false;
bool Game::muted = false;
bool Game::paused = false;
SDL_Renderer* Game::renderer = nullptr;

int main(int argc, char* argv[]) {
    Game game;
    while (game.isOpen) game.loop();
    return 0;
}
