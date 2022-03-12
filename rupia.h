#pragma once
#include "cuadrado.h"
class Rupia : public Cuadrado {
	public:
		int valor = 1;
		Rupia() {
			Cuadrado::Cuadrado();
			srcRect = NULL;
			dstRect = new SDL_Rect();
		};
};