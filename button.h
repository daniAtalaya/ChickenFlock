#pragma once
#include "general.h"
#include "cuadrado.h"
class Button : public Cuadrado {
	public:
		Button() {
			Cuadrado::Cuadrado();
		};

		bool isClicked(SDL_Rect*);
};

