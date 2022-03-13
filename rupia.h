#pragma once
#include "cuadrado.h"
#include "general.h"
class Rupia : public Cuadrado {
	public:
		int valor = 1;
		int tipus;
		Rupia() {
			Cuadrado::Cuadrado();
			tipus = R_NUM(1, 4);
			srcRect = NULL;
			dstRect = new SDL_Rect();
		};
};