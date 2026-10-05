#pragma once
#include "color.h"
#include "general.h"

class Cuadrado {
	public:
		Cuadrado() = default;
		SDL_Rect* dstRect = nullptr;
		SDL_Rect* srcRect = nullptr;
		Color color;
		int sX = 5;
		mutable bool disposable = false;
		int sY = 5; 
		SDL_Texture* img = nullptr;
		void draw() const;
		void drawHitbox() const;
		void update(int, int) const;
		void update() const;
		bool checkCollision(const SDL_Rect*) const;
};