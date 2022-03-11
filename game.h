#pragma once
#include "general.h"
#include "cuadrado.h"
#include "almacen.h"
class Game {
	public:
		Game();
		~Game();
		void loop();
		static bool god;
		static bool paused;
		bool isOpen = false;
		static SDL_Renderer* renderer;
	private:
		void input();
		void update();
		void draw();
		bool load();
		Almacen<SDL_Texture*> images;
		Almacen<Mix_Music*> tracks;
		Almacen<Mix_Chunk*> sfxs;
		Cuadrado fondo;
		SDL_Event event;
		const Uint8* keyboard;
		SDL_Window* window;
};