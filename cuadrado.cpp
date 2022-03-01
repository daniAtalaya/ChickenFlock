#include "cuadrado.h"
void Cuadrado::move(int f1, int f2) {
	rect.x += f1 ? 0 : f2 ? sX : -sX;
	rect.y += f1 ? f2 ? sY : -sY : 0;
	if (rect.y + rect.h < 0) rect.y = WINDOW_H - rect.h;
	if (rect.y + rect.h > WINDOW_H) rect.y = 0;
	if (rect.x + rect.w < 0) rect.x = WINDOW_W - rect.w;
	if (rect.x + rect.w > WINDOW_W) rect.x = 0;
}