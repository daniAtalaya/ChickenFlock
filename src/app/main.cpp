#include "game.h"
#include "general.h"
#include "enums.h"

bool Game::god = false;
bool Game::muted = false;
bool Game::paused = false;
Escena Game::escena = INICI;
SDL_Renderer* Game::renderer = nullptr;

int main(int argc, char* argv[]) {
    Game game;
    while (game.isOpen) game.loop();
    return 0;
}
