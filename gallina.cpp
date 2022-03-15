#include "gallina.h"
void Gallina::init(SDL_Texture* image) {
	img = image;
	spritesheet.setSpritesheet(img, 7, 4);
	srcRect->w = spritesheet.frameW;
	srcRect->x = srcRect->y = 0;
	srcRect->h = spritesheet.frameH;
	dstRect->w = 70;
	dstRect->h = 70;
}
