#include "game_renderer.h"

void GameRenderer::beginFrame(SDL_Renderer* renderer) {
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
}

void GameRenderer::endFrame(SDL_Renderer* renderer) {
	SDL_RenderPresent(renderer);
}
