#pragma once
#include "general.h"
#include "color.h"

class Cuadrado {
	public:
		Cuadrado() = default;
		SDL_Rect* dstRect = nullptr;
		SDL_Rect* srcRect = nullptr;
		Color color;
		int sX = 5;
		bool disposable = false;
		int sY = 5; 
		SDL_Texture* img = nullptr;
		void draw();
		void drawHitbox();
		void update(int, int);
		void update();
		bool checkCollision(SDL_Rect*);
};