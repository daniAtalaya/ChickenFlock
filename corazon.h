#pragma once
#include "cuadrado.h"
class Corazon : public Cuadrado {
	public:
		SDL_Texture* alive;
		SDL_Texture* dead;
		Corazon() {};
		Corazon(int i) {
			Cuadrado::Cuadrado();
			srcRect = NULL;
			dstRect = new SDL_Rect();
			dstRect->x = WINDOW_W - 60 - i * 50;
			dstRect->y = 20;
			dstRect->w = dstRect->h = 40;
		}
};