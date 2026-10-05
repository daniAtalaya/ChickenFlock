#include "button.h"
bool Button::isClicked(SDL_Rect* mouse) {
	return SDL_HasIntersection(dstRect, mouse);
};
