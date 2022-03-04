#include "jugador.h"

void Jugador::update() {
	rect.x += sX;
	rect.y += sY;
	//If the player goes beyond screen upper limits, teleport to the bottom
	if (rect.y < 0) rect.y = WINDOW_H - rect.h;
	//If the player goes beyond screen lower limits, teleport to the top
	if (rect.y + rect.h > WINDOW_H) rect.y = 0;
}