#include "cuadrado.h"
#include "general.h"
#include "game.h"

void Cuadrado::draw() {
	SDL_SetRenderDrawColor(Game::renderer, color.r, color.g, color.b, color.a);
	if(img == NULL) SDL_RenderFillRect(Game::renderer, dstRect);
	if(img != NULL) SDL_RenderCopy(Game::renderer, img, srcRect, dstRect);
	if (Game::god) drawHitbox();
}

void Cuadrado::update(int dx, int dy) {
	dstRect->x += dx * sX;
	dstRect->y += dy * sY;
}
void Cuadrado::drawHitbox() {
	SDL_SetRenderDrawColor(Game::renderer, 192, 0, 0, 255);
	SDL_RenderDrawRect(Game::renderer, dstRect);
}