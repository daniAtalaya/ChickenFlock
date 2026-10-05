#pragma once
#include "general.h"
#include "cuadrado.h"

class Button : public Cuadrado {
	public:
		Button() {
			Cuadrado();
		};

		bool isClicked(const SDL_Rect*) const;
};