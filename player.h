#pragma once
#include "general.h"
#include "cuadrado.h"
class Player : public Cuadrado
{
	public:
		Player() {
			Cuadrado::Cuadrado();
		}
		void update(int dx, int dy) {
			Cuadrado::update(dx, dy);
		}
		void draw() {
			Cuadrado::draw();

			std::cout << "PLAYER DRAWN" << std::endl;
		}
};

