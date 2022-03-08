#include "bala.h"
#include "window.h"
#include "general.h"

Bala::Bala(SDL_Rect ref){
	rect.w = 30;
	rect.h = 10;
	sX = 10;
	sY = 0;
	rect.x = ref.x + ref.w;
	rect.y = ref.y + (ref.h / 2) - (rect.h / 2);
}

void Bala::draw(SDL_Renderer *rend, SDL_Texture *img, bool godMode) {
	SDL_RenderCopy(rend, img, NULL, &rect);
	if (godMode) SDL_RenderDrawRect(rend, &rect);
}