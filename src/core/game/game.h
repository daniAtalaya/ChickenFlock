#pragma once
#include "general.h"
#include "player/player.h"
#include "enums.h"
#include "gameplay/camera.h"
#include "resources/game_assets.h"
#include "game_scene_manager.h"
#include "game_progress.h"
#include "save_data_store.h"

class Game {
	public:
		Game();
		~Game();
		void loop();
		bool isOpen = false;
		private:
			bool muted = false;
			bool debugHitboxes = false;
			void init();
			void update();
		void input();
		void draw() const;
		void mute();
		void pause();
		void playSound(const std::string&, int);
		void playMusic(const std::string&, int);
		void haltMusic();
		void haltChannels();
		GameSceneManager::SceneCollection createScenes();
		void initializeLevel();
		void initializePlayer();
		bool persistenceReady = false;
		void cambiaEscena(Escena);
		SDL_Renderer* renderer = nullptr;
		GameProgress progress;
		SaveDataStore saveDataStore;
		Camera camera;
		Player player;
		SDL_Window* window = nullptr;
		const Uint8* keyboard = nullptr;
		GameAssets assets;
		GameSceneManager sceneManager;

};
