#include "spritesheet.h"

void SpriteSheet::setSpritesheet(SDL_Texture* img, const int mF, const int mC) {
	maxC = mC;
	maxF = mF;
	currentImage = img;
	SDL_QueryTexture(currentImage, nullptr, nullptr, &textureW, &textureH);
	frameW = textureW / mC;
	frameH = textureH / mF;
}