#pragma once
#include "cuadrado.h"
#include "general.h"
#include "spritesheet.h"
class Gallina : public Cuadrado {
	public:
		SpriteSheet spritesheet;
		int index = 0;
		int tipus = 1;
		int direccion = 1;
		void update() {
			dstRect->x += R_NUM(-5, 5);
			dstRect->y += sY;
		}
		Gallina() {
			Cuadrado::Cuadrado();
			srcRect = new SDL_Rect();
			dstRect = new SDL_Rect();
		}
		void init(SDL_Texture*);
		void animateY() {
			direccion = R_NUM(0, spritesheet.maxF - 1);
			srcRect->y = spritesheet.frameH * direccion;
		}
		void animateX() {
			srcRect->x = spritesheet.frameW * index;
			if (++index > spritesheet.maxC - 1) index = 0;
		}
};
