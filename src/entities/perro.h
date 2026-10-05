#pragma once
#include "spritesheet.h"
#include "cuadrado.h"
class Perro : public Cuadrado {
public:
	SpriteSheet spritesheet;
	int index = 0;
	int direccion = 0;
	bool isPresent = false;
	Perro() {
		Cuadrado::Cuadrado();
		srcRect = new SDL_Rect();
		dstRect = new SDL_Rect();
	}
	void init(SDL_Texture*); 
	void animateY() {
		//if (++direccion >= spritesheet.maxF) direccion = 0;
		srcRect->y = spritesheet.frameH * direccion;
	}
	void animateX() {
		srcRect->x = spritesheet.frameW * index;
		if (++index >= spritesheet.maxC) index = 0;
	}
};