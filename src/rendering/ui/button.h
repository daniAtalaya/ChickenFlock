#pragma once
#include "general.h"
#include "base/cuadrado.h"

class Button : public Cuadrado {
	public:
		Button() {
			Cuadrado();
		};

		bool isClicked(const SDL_Point&) const;
};