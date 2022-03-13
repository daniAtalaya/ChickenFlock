#pragma once
#include "general.h"
#include "camera.h"
#include "button.h"
#include "almacen.h"
#include "player.h"
#include "gallina.h"
#include "rupia.h"
#include "enums.h"
#include "rupia.h"
class Game {
	public:
		Game();
		~Game();
		void loop();
		static bool god;
		static bool muted;
		static bool paused;
		static Escena escena;
		bool isOpen = false;
		static SDL_Renderer* renderer;
	private:
		void init();
		void assignImg();
		void destroy();
		void update();
		void input();
		void draw();
		bool load();
		void cambiaEscena(Escena);
		bool showPausaText = true;
		Button botonSonido;
		Button botonPlay;
		Button botonShop;
		Cuadrado nivel;
		Cuadrado horda;
		Camera camera;
		Player player;
		SDL_Event event;
		SDL_Window* window;
		const Uint8* keyboard;
		Almacen<SDL_Texture*> images;
		Almacen<Mix_Music*> tracks;
		Almacen<Mix_Chunk*> sfxs;
		std::vector<Cuadrado> obstaculos;
		std::vector<Rupia> rupias;
		std::vector<Gallina> gallinas;
};