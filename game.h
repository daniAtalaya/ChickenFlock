#pragma once
#include "general.h"
#include "cuadrado.h"
#include "almacen.h"
class Game {
	public:
		Game();
		~Game();
		void input();
		void update();
		void draw();
		void loop();
		bool isOpen = false;
		static bool god;
		static bool paused;
		Almacen<SDL_Texture*> images;
		Almacen<Mix_Music*> tracks;
		Almacen<Mix_Chunk*> sfxs;
		Cuadrado fondo;
		SDL_Event event;
		const Uint8* keyboard;
		SDL_Window* window = nullptr;
		static SDL_Renderer* renderer;
};