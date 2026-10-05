#include "camera.h"
#include "game.h"

void Camera::update() const {
	srcRect->y -= sY;
}