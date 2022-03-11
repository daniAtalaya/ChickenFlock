#pragma once
#include "general.h"
#include "color.h"

class Cuadrado {
	public:
		Cuadrado() = default;
		SDL_Rect rect;
		Color color;
		int sX = 5;
		int sY = 5;
		void draw();
		void drawHitbox();
		void update(int, int);
		void draw(SDL_Texture*);
};
