#pragma once
#include "general.h"
#include "cuadrado.h"
class Jugador : public Cuadrado
{
	public:
		void update(int, int);
		void draw(SDL_Renderer*);
		Jugador() { cR = 255; }
};