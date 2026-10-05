#pragma once
#include "cuadrado.h"
class Roca : public Cuadrado {
public:
	Roca() {
		Cuadrado::Cuadrado();
		srcRect = NULL;
		dstRect = new SDL_Rect();
	};
};

