#pragma once
#include "cuadrado.h"
#include "general.h"
#include "spritesheet.h"
class Pajaro : public Cuadrado {
	public:
		SpriteSheet spritesheet;
		int index = 0;
		int direccion = 1;
		Pajaro() {
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
			if (++index > spritesheet.maxF) index = 0;
		}
};

