#pragma once
#include "base/cuadrado.h"

class Camera : public Cuadrado {
	public:
		Camera() {
			Cuadrado();
		}

		void update() const;
};