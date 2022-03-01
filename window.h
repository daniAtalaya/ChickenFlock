#pragma once
#include "general.h"
#include "cuadrado.h"
class Window {
	public:
		Window();
		~Window();
		void input();
		void update();
		void draw();
		bool isOpen = false;
		Cuadrado player;
	private:
		SDL_Event _event;
		const Uint8* _keyboard;
		SDL_Window* _window = nullptr;
		SDL_Renderer* _renderer = nullptr;
};