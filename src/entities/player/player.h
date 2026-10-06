#pragma once
#include "general.h"
#include "base/cuadrado.h"
#include "base/spritesheet.h"
#include "health/corazon.h"

class Player : public Cuadrado {
	public:
		SpriteSheet spritesheet;
		int direccion = 1; 
		int index = 0;
		int vides = 3;
		Corazon corazones[3];

		Player() {
			Cuadrado::Cuadrado();
			srcRect = new SDL_Rect();
			srcRect->x = 0;
			for (int i = 0; i < 3; i++) corazones[i] = Corazon(i);
		}

		void init(SDL_Texture*);

		bool damage() {
			if (vides <= 0 || isInvulnerable()) {
				return false;
			}
			--vides;
			lastDamageTick = SDL_GetTicks();
			hasTakenDamage = true;
			corazones[vides].img = corazones[vides].dead;
			return true;
		}

		bool isInvulnerable() const {
			return hasTakenDamage && SDL_GetTicks() - lastDamageTick < damageInvulnerabilityMs;
		}

		void animateY() const {
			srcRect->y = spritesheet.frameH * direccion;
		}

		void animateX() {
			srcRect->x = spritesheet.frameW * index;
			if (++index >= spritesheet.maxC) index = 0;
		}

	private:
		static constexpr Uint32 damageInvulnerabilityMs = 1000;
		Uint32 lastDamageTick = 0;
		bool hasTakenDamage = false;
};