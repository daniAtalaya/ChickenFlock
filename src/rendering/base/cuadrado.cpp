#include "cuadrado.h"
#include "general.h"
#include "game/game.h"

void Cuadrado::draw() const {
	if (dstRect == nullptr) return;
	SDL_SetRenderDrawColor(Game::renderer, color.r, color.g, color.b, color.a);
	if (img != nullptr) {
		SDL_RenderCopy(Game::renderer, img, srcRect, dstRect);
	}
	if (Game::god) {
		drawHitbox();
	}
}

void Cuadrado::update(const int dx, const int dy) const {
	if (dstRect == nullptr) {
		return;
	}
	dstRect->x += dx * sX;
	dstRect->y += dy * sY;
}

void Cuadrado::update() const {
	if (dstRect == nullptr) {
		return;
	}
	dstRect->x += sX;
	dstRect->y += sY;
}

void Cuadrado::drawHitbox() const {
	if (dstRect == nullptr) {
		return;
	}
	SDL_SetRenderDrawColor(Game::renderer, 192, 0, 0, 255);
	SDL_RenderDrawRect(Game::renderer, dstRect);
}

bool Cuadrado::checkCollision(const SDL_Rect* otherRect) const {
	return dstRect != nullptr && otherRect != nullptr && SDL_HasIntersection(dstRect, otherRect);
}