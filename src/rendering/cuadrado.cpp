#include "cuadrado.h"
#include "general.h"
#include "game.h"

void Cuadrado::draw() {
	if (dstRect == nullptr) return;
	SDL_SetRenderDrawColor(Game::renderer, color.r, color.g, color.b, color.a);
	//if(img == NULL) SDL_RenderFillRect(Game::renderer, dstRect);
	if(img != NULL) SDL_RenderCopy(Game::renderer, img, srcRect, dstRect);
	if (Game::god) drawHitbox();
}

void Cuadrado::update(int dx, int dy) {
	if (dstRect == nullptr) return;
	dstRect->x += dx * sX;
	dstRect->y += dy * sY;
}
void Cuadrado::update() {
	if (dstRect == nullptr) return;
	dstRect->x += sX;
	dstRect->y += sY;
}
void Cuadrado::drawHitbox() {
	if (dstRect == nullptr) return;
	SDL_SetRenderDrawColor(Game::renderer, 192, 0, 0, 255);
	SDL_RenderDrawRect(Game::renderer, dstRect);
}
bool Cuadrado::checkCollision(SDL_Rect* otherRect) {
	return dstRect != nullptr && otherRect != nullptr && SDL_HasIntersection(dstRect, otherRect);
}