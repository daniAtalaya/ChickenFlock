#pragma once
#include "general.h"
#include "color.h"

class Cuadrado {
	public:
		Cuadrado() = default;
		SDL_Rect* dstRect;
		SDL_Rect* srcRect;
		Color color;
		int sX = 5;
		int sY = 5; 
		SDL_Texture* img;
		void draw();
		void drawHitbox();
		void update(int, int);
};