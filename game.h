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
#include "perro.h"
#include "arbol.h"
#include "roca.h"
#include "flecha.h"
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
		int partidesJugades = 0;
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
		bool ostrichShown = false;
		bool creditsShown = false;
		void cambiaEscena(Escena);
		bool showPausaText = true;
		Button botonSonido; 
		int loreShown = 0;
		int loreTienda = 0;
		int dineroTemporal = 0;
		int tipoGallinaTrasera=0;
		Button botonPlay;
		Button botonShop;
		Button botonBack;
		Button botonExitShop;
		Button botonCompraAzul;
		Button botonCompraGolden;
		Button botonCompraDark;
		Button botonCompraBrown;
		Button botonHardcore;
		Button botonCreditos;
		Cuadrado nivel;
		Cuadrado horda;
		Pajaro pajaro;
		Perro mascota;
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
		std::vector<Arbol*> arboles;
		std::vector<Roca*> rocas;
		std::vector<Flecha*> flechas;
		Cuadrado paredHitboxLeft;
		Cuadrado paredHitboxRight;
		Cuadrado creditos;
		Cuadrado continuara;
		//Avestruz avestruz;
		//Cuadrado rioHitboxLeft;
		//Cuadrado rioHitboxRight;
};
