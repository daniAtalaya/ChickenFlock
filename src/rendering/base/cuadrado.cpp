#include "cuadrado.h"
#include "general.h"

void Cuadrado::draw(SDL_Renderer* renderer, const bool showHitboxes) const {
	if (dstRect == nullptr) return;
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	if (img != nullptr) {
		SDL_RenderCopy(renderer, img, srcRect, dstRect);
	}
	if (showHitboxes) {
		drawHitbox(renderer);
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

void Cuadrado::drawHitbox(SDL_Renderer* renderer) const {
	if (dstRect == nullptr) {
		return;
	}
	SDL_SetRenderDrawColor(renderer, 192, 0, 0, 255);
	SDL_RenderDrawRect(renderer, dstRect);
}

bool Cuadrado::checkCollision(const SDL_Rect* otherRect) const {
	return dstRect != nullptr && otherRect != nullptr && SDL_HasIntersection(dstRect, otherRect);
}