#pragma once
#include "general.h"
#include "ui/button.h"
#include "player/player.h"
#include "enums.h"
#include "easter/avestruz/avestruz.h"
#include "gameplay/camera.h"
#include "npc/pajaro/pajaro.h"
#include "npc/perro/perro.h"
#include "resources/game_assets.h"
#include "game_world.h"
#include "game_scene_factory_context.h"
#include "game_scene_manager.h"

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
		void update();
		void input();
		void draw();
		void mute();
		void pause();
		void playSound(const std::string&, int);
		void playMusic(const std::string&, int);
		void haltMusic();
		void haltChannels();
		GameSceneFactoryContext sceneFactoryContext();
		bool hardMode = false;
		void cambiaEscena(Escena);
		Button botonSonido; 
		int dineroTemporal = 0;
		int tipoGallinaTrasera=0;
		Button botonPlay;
		Button botonShop;
		Button botonBack;
		Button botonHardcore;
		Cuadrado nivel;
		Cuadrado horda;
		Pajaro pajaro;
		Perro mascota;
		Camera camera;
		Player player;
		SDL_Event event;
		SDL_Window* window = nullptr;
		const Uint8* keyboard = nullptr;
		Avestruz avestruz;
		GameAssets assets;
		GameWorld world;
		GameSceneManager sceneManager;
		Cuadrado paredHitboxLeft;
		Cuadrado paredHitboxRight;
		Cuadrado creditos;
		Cuadrado continuara;

};
