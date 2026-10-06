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
		void draw(SDL_Renderer* renderer, bool showHitboxes = false) const;
		void drawHitbox(SDL_Renderer* renderer) const;
		void update(int, int) const;
		void update() const;
		bool checkCollision(const SDL_Rect*) const;
};