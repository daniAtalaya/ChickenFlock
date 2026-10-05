#pragma once
#include "base/cuadrado.h"

class Flecha : public Cuadrado {
	public:
		Flecha() {
			Cuadrado();
			srcRect = nullptr;
			dstRect = new SDL_Rect();
		};
};