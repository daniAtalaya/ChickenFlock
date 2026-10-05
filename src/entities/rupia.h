#pragma once
#include "cuadrado.h"
#include "general.h"

class Rupia : public Cuadrado {
	public:
		int tipus;
		int valor;

		Rupia() : valor(0) {
			Cuadrado();
			tipus = R_NUM(1, 4);
			srcRect = nullptr;
			dstRect = new SDL_Rect();
		}
};