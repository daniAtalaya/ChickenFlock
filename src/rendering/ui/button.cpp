#include "button.h"

bool Button::isClicked(const SDL_Rect* mouse) const {
	return dstRect != nullptr && mouse != nullptr && SDL_HasIntersection(dstRect, mouse);
}