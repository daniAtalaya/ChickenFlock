#pragma once
#include "cuadrado.h"
class Camera : public Cuadrado {
	public:
		Camera() {
			Cuadrado::Cuadrado();
		}
		void update();
};

