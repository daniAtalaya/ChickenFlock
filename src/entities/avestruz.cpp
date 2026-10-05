#include "avestruz.h"
void Avestruz::init(SDL_Texture* image) {
	img = image;
	spritesheet.setSpritesheet(img, 5, 4);
	if (srcRect == nullptr) srcRect = new SDL_Rect();
	srcRect->w = spritesheet.frameW;
	srcRect->x = srcRect->y = 0;
	srcRect->h = spritesheet.frameH;
	dstRect->w = 85;
	dstRect->h = 85;
}