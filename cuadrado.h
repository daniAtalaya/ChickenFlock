#pragma once
#include "general.h"

class Cuadrado {
	public:
		//Empty constructor for the player
		Cuadrado() = default;
					//     X    Y    W  H
		SDL_Rect rect;
		int sX = 5;
		int sY = 5;
		int cR = 0;
		int cG = 0;
		int cB = 0;
		int cA = 0;
		void update(int, int);
		void draw(SDL_Renderer*);
	private:
};
