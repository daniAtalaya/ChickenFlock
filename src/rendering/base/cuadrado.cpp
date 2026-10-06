#include "cuadrado.h"
#include "general.h"
#include <algorithm>

void Cuadrado::draw(SDL_Renderer* renderer, const bool showHitboxes) const {
	if (dstRect == nullptr) return;
	SDL_SetRenderDrawColor(
		renderer,
		static_cast<Uint8>(std::clamp(color.r, 0, 255)),
		static_cast<Uint8>(std::clamp(color.g, 0, 255)),
		static_cast<Uint8>(std::clamp(color.b, 0, 255)),
		static_cast<Uint8>(std::clamp(color.a, 0, 255))
	);
	if (img != nullptr) {
		renderTexture(renderer, img, srcRect, dstRect);
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
	drawRect(renderer, *dstRect);
}

bool Cuadrado::checkCollision(const SDL_Rect* otherRect) const {
	return dstRect != nullptr && otherRect != nullptr && SDL_HasRectIntersection(dstRect, otherRect);
}