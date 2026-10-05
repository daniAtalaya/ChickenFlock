#include "button.h"
bool Button::isClicked(SDL_Rect* mouse) {
	return dstRect != nullptr && mouse != nullptr && SDL_HasIntersection(dstRect, mouse);
};
