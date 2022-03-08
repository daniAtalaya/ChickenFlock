#include "cuadrado.h"
#include "window.h"

void Cuadrado::draw(SDL_Renderer* rend) {
	SDL_SetRenderDrawColor(rend, cR, cG, cB, cA);
	SDL_RenderFillRect(rend, &rect);
}

void Cuadrado::update(int dx, int dy)
{
	rect.x += dx * sX;
	rect.y += dy * sY;
}