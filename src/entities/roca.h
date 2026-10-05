#pragma once
#include "cuadrado.h"

class Roca : public Cuadrado {
public:
	Roca() {
		Cuadrado();
		srcRect = NULL;
		dstRect = new SDL_Rect();
	};
};