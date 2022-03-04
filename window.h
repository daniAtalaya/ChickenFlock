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
		void destroy();
		bool isOpen = false;
		std::vector<Bala> bullets;
		Jugador player;
	private:
		SDL_Event event;
		const Uint8* keyboard;
		SDL_Window* window = nullptr;
		SDL_Renderer* renderer = nullptr;
};