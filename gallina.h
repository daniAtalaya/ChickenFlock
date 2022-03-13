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
		Gallina() {
			Cuadrado::Cuadrado();
			srcRect = NULL;
			dstRect = new SDL_Rect();
		}
		void init(SDL_Texture*);
		void animateY() {
			srcRect->y = spritesheet.frameH * direccion;
		}
		void animateX() {
			srcRect->x = spritesheet.frameW * index;
			if (++index > spritesheet.maxC) index = 0;
		}
};
