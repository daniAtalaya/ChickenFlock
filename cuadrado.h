#pragma once
#include "general.h"

class Cuadrado {
	public:
		//Empty constructor for the player
		Cuadrado() = default;
					//     X    Y    W  H
		SDL_Rect rect = { 100, 100, 120, 70 };
		int sX = 10;
		int sY = 10;
		int cR = 0;
		int cG = 0;
		int cB = 0;
		int cA = 0;
		bool disposable = false;
		void draw(SDL_Renderer*);
	private:
};
