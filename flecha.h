#pragma once
#include "cuadrado.h"
class Flecha : public Cuadrado {
	public:
		Flecha() {
			Cuadrado::Cuadrado();
			srcRect = NULL;
			dstRect = new SDL_Rect();
		};
};
