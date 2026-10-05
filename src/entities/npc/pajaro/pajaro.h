#pragma once
#include "general.h"
#include "base/cuadrado.h"
#include "base/spritesheet.h"

class Pajaro : public Cuadrado {
	public:
		SpriteSheet spritesheet;
		int index = 0;
		int direccion = 0;
		bool isPresent = false;

		Pajaro() {
			Cuadrado();
			srcRect = new SDL_Rect();
			dstRect = new SDL_Rect();
		}

		void init(SDL_Texture*);

		void animateX() {
			srcRect->x = spritesheet.frameW * index;
			if (++index >= spritesheet.maxC) index = 0;
		}
};
