#pragma once
#include "general.h"
class Cuadrado {
	public:
		Cuadrado() = default;
		SDL_Rect rect = { 100, 100, 50, 50 };
		int cR = 0;
		int cG = 128;
		int cB = 0;
		int cA = 0;
		void move(int, int);
	private:
		int sX = 10;
		int sY = 10;
};

