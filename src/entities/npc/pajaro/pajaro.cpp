#include "pajaro.h"
void Pajaro::init(SDL_Texture* image) {
	img = image;
	spritesheet.setSpritesheet(img, 1, 10);
	srcRect->w = spritesheet.frameW;
	srcRect->h = spritesheet.frameH;
	dstRect->w = 85;
	dstRect->h = 85;
	srcRect->y = spritesheet.frameH * direccion;
}