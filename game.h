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
		int loreShown = 0;
		Button botonPlay;
		Button botonShop;
		Button botonBack;
		Cuadrado nivel;
		Cuadrado horda;
		Camera camera;
		Player player;
		SDL_Event event;
		SDL_Window* window;
		const Uint8* keyboard;
		Almacen<Mix_Chunk*> sfxs;
		std::vector<Rupia> rupias;
		Almacen<Mix_Music*> tracks;
		Almacen<SDL_Texture*> images;
		std::vector<Gallina> gallinas;
		std::vector<Cuadrado> obstaculos;
		std::vector<Cuadrado> hitboxes;
};