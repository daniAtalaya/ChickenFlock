#pragma once
#include "general.h"
#include "jugador.h"
#include "bala.h"
class Window {
	public:
		Window();
		~Window();
		void input();
		void update();
		void draw();
		void loop();
		void destroyBullets();
		bool isOpen = false;
		bool godMode;
		Jugador temp;
		SDL_Texture* img_background;
		SDL_Texture* img_player;
		SDL_Texture* img_shot;
		std::vector<Bala> bullets;
		Jugador player;
		Cuadrado fondo;
	private:
		SDL_Event event;
		const Uint8* keyboard;
		SDL_Window* window = nullptr;
		SDL_Renderer* renderer = nullptr;
};