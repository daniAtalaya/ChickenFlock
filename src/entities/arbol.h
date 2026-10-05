#pragma once
#include "cuadrado.h"
class Arbol : public Cuadrado {
	public:
		Arbol() {
			Cuadrado::Cuadrado();
			srcRect = NULL;
			dstRect = new SDL_Rect();
		};
};

