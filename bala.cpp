#include "bala.h"
#include "general.h"

Bala::Bala(SDL_Rect ref){
	cG = 255;
	rect.w = 30;
	rect.h = 10;
	sX = 10;
	sY = 0;
	rect.x = ref.x + ref.w;
	rect.y = ref.y + (ref.h / 2) - (rect.h / 2);
}
void Bala::update() {
	if (!disposable) {
		rect.x += sX;
		rect.y += sY;
		disposable = rect.x < -200 || rect.x > WINDOW_W + 200 || rect.y < -200 || rect.y > WINDOW_H + 200;
	}
}