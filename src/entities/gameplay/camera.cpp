#include "camera.h"
#include "game/game.h"

void Camera::update() const {
	srcRect->y -= sY;
}