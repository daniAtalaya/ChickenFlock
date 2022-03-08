#pragma once

#include "general.h"
#include "cuadrado.h"
class Bala : public Cuadrado
{
	public:
		Bala(SDL_Rect);
		void draw(SDL_Renderer*, SDL_Texture*, bool);
};

