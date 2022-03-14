#pragma once
#include "general.h"
#include "camera.h"
#include "button.h"
#include "almacen.h"
#include "player.h"
#include "gallina.h"
#include "rupia.h"
#include "enums.h"
#include "pajaro.h"
#include "avestruz.h"
class Game {
	public:
		Game();
		~Game();
		void loop();
		static bool god;
		static bool muted;
		static bool paused;
		SDL_Rect* mouse = new SDL_Rect({1, 1, 10, 10});
		static Escena escena;
		bool isOpen = false;
		bool isClicking = false;
		static SDL_Renderer* renderer;
	private:
		void init();
		void assignImg();
		void destroy();
		void update();
		void input();
		void draw();
		bool load();
		void mute();
		void pause();
		bool hardMode = false;
		void cambiaEscena(Escena);
		bool showPausaText = true;
		bool endingReached = false;
		Button botonSonido; 
		int loreShown = 0;
		int loreTienda = 0;
		Button botonPlay;
		Button botonShop;
		Button botonBack;
		Button botonExitShop;
		Button botonCompraAzul;
		Button botonCompraGolden;
		Button botonCompraDark;
		Button botonCompraBrown;
		Button botonHardcore;
		Cuadrado nivel;
		Cuadrado horda;
		Pajaro pajaro;
		Camera camera;
		Player player;
		SDL_Event event;
		SDL_Window* window;
		const Uint8* keyboard;
		Almacen<Mix_Chunk*> sfxs;
		Avestruz avestruz;
		std::vector<Rupia*> rupias;
		Almacen<Mix_Music*> tracks;
		Almacen<SDL_Texture*> images;
		std::vector<Gallina*> gallinas;
		std::vector<Cuadrado*> obstaculos;
		std::vector<Cuadrado*> flechas;
		Cuadrado paredHitboxLeft;
		Cuadrado paredHitboxRight;
		//Cuadrado rioHitboxLeft;
		//Cuadrado rioHitboxRight;
};
