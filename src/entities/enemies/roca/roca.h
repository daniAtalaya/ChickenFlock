#pragma once
#include "base/cuadrado.h"

class Roca : public Cuadrado {
public:
	Roca() {
		Cuadrado();
		srcRect = nullptr;
		dstRect = new SDL_Rect();
	};
};