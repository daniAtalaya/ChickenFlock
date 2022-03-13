#pragma once
#include "general.h"
#include "cuadrado.h"
#include "spritesheet.h"
#include "corazon.h"
class Player : public Cuadrado {
	public:
		SpriteSheet spritesheet;
		int direccion = 1; 
		int index = 0;
		int vides = 3;
		int money = 0;
		bool brownComprada = false;
		bool azulComprada = false;
		bool darkComprada = false;
		bool goldenComprada = false;
		int gallinasDesbloqueadas = 1;
		Corazon corazones[3];
		Player() {
			Cuadrado::Cuadrado();
			srcRect = new SDL_Rect();
			srcRect->x = 0;
			for (int i = 0; i < 3; i++) corazones[i] = Corazon(i);
		}
		void init(SDL_Texture*);
		void update(int dx, int dy) {
			Cuadrado::update(dx, dy);
		}
		void damage() {
			if (vides-- > 0) {
				corazones[vides].img = corazones[vides].dead;
			}
		}
		void animateY(){
			srcRect->y = spritesheet.frameH * direccion;
		}
		void animateX() {
			srcRect->x = spritesheet.frameW * index;
			if (++index >= spritesheet.maxC) index = 0;
		}
};

