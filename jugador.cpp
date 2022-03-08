#include "jugador.h"

void Jugador::update(int dX, int dY) {
	Cuadrado::update(dX, dY);
	//If the player goes beyond screen upper limits, teleport to the bottom
	if (rect.y < -rect.h) rect.y = WINDOW_H - rect.h;
	//If the player goes beyond screen lower limits, teleport to the top
	if (rect.y > WINDOW_H) rect.y = 0;
}

void Jugador::draw(SDL_Renderer* rend) {
	SDL_SetRenderDrawColor(rend, cR, cG, cB, cA);
	SDL_RenderFillRect(rend, &rect);
}