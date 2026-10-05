#pragma once

#include "general.h"

class GameRenderer {
public:
	static void beginFrame(SDL_Renderer* renderer);
	static void endFrame(SDL_Renderer* renderer);
};
