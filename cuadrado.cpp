#include "cuadrado.h"
#include "window.h"

//Parametrized colors with the (RGB)an't color values

void Cuadrado::draw(SDL_Renderer* rend) {
	if (!disposable) {
		SDL_SetRenderDrawColor(rend, cR, cG, cB, cA);
		SDL_RenderFillRect(rend, &rect);
	}
}