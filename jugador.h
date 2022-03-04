#pragma once
#include "general.h"
#include "cuadrado.h"
class Jugador : public Cuadrado
{
	public:
		void update();
		Jugador() { cR = 255; }
};