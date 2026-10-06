#include "button.h"

bool Button::isClicked(const SDL_Point& position) const {
	return dstRect != nullptr && SDL_PointInRect(&position, dstRect);
}