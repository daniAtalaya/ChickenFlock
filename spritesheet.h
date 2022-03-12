#pragma once
#include "general.h"
class SpriteSheet
{
	public:
		int frameW, frameH;
		int textureW, textureH;
		SDL_Texture* currentImage;
		int maxC;
		int maxF;
		SpriteSheet() {};
		void setSpritesheet(SDL_Texture* img, int mF, int mC);
		
};

