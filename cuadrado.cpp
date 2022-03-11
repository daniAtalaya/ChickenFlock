#include "cuadrado.h"
#include "general.h"
#include "game.h"

void Cuadrado::draw() {
	SDL_SetRenderDrawColor(Game::renderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(Game::renderer, &rect);
	if (Game::god) drawHitbox();
}

void Cuadrado::draw(SDL_Texture* img) {
	SDL_RenderCopy(Game::renderer, img, NULL, &rect);
	if (Game::god) drawHitbox();
}

void Cuadrado::update(int dx, int dy) {
	rect.x += dx * sX;
	rect.y += dy * sY;
}
void Cuadrado::drawHitbox() {
	SDL_SetRenderDrawColor(Game::renderer, 192, 0, 0, 255);
	SDL_RenderDrawRect(Game::renderer, &rect);
}