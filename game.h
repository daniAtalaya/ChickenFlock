#pragma once
#include "general.h"
#include "camera.h"
#include "button.h"
#include "almacen.h"
#include "player.h"
class Game {
	public:
		Game();
		~Game();
		void loop();
		static bool god;
		static bool muted;
		static bool paused;
		bool isOpen = false;
		static SDL_Renderer* renderer;
	private:
		void things();
		void update();
		void input();
		void draw();
		bool load();
		Button botonSonido;
		Cuadrado nivel;
		Camera camera;
		Player player;
		SDL_Event event;
		SDL_Window* window;
		const Uint8* keyboard;
		Almacen<SDL_Texture*> images;
		Almacen<Mix_Music*> tracks;
		Almacen<Mix_Chunk*> sfxs;
};