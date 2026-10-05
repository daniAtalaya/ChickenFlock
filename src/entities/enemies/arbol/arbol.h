#pragma once
#include "base/cuadrado.h"

class Arbol : public Cuadrado {
	public:
		Arbol() {
			Cuadrado();
			srcRect = nullptr;
			dstRect = new SDL_Rect();
		};
};