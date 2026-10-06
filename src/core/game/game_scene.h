#pragma once

#include "enums.h"
#include <SDL3/SDL.h>

class GameScene {
	public:
		virtual ~GameScene() = default;
		virtual Escena id() const = 0;
		virtual void enter(Escena) {}
		virtual void exit(Escena) {}
		virtual void handleInput(const SDL_Event&) {}
		virtual void handleClick(const SDL_Point&) {}
		virtual void update() {}
		virtual void render(SDL_Renderer*, bool showHitboxes) = 0;
};