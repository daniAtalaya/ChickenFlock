#pragma once
#include "cuadrado.h"

class Flecha : public Cuadrado {
	public:
		Flecha() {
			Cuadrado();
			srcRect = nullptr;
			dstRect = new SDL_Rect();
		};
};