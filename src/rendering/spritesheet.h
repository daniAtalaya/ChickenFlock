#pragma once
#include "general.h"
class SpriteSheet {
	public:
		int frameW = 0;
		int frameH = 0;
		int textureW = 0;
		int textureH = 0;
		SDL_Texture* currentImage = nullptr;
		int maxC = 0;
		int maxF = 0;
		SpriteSheet() = default;
		void setSpritesheet(SDL_Texture* img, int mF, int mC);
};
