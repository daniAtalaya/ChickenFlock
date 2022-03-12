#include "spritesheet.h"
void SpriteSheet::setSpritesheet(SDL_Texture* img, int mF, int mC) {
	maxC = mC;
	maxF = mF;
	currentImage = img;
	SDL_QueryTexture(currentImage, NULL, NULL, &textureW, &textureH);
	frameW = textureW / mC;
	frameH = textureH / mF;
}