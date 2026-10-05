#pragma once
#include "base/cuadrado.h"
#include "general.h"
#include "base/spritesheet.h"

class Avestruz : public Cuadrado {
	public:
		SpriteSheet spritesheet;
		int index = 0;
		int direccion = 1;
		Avestruz() {
			Cuadrado();
			srcRect = nullptr;
			dstRect = new SDL_Rect({0, 0, 0, 0});
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