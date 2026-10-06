#pragma once
#include "player/player.h"
#include "enums.h"
#include "gameplay/camera.h"
#include "resources/game_assets.h"
#include "game_scene_manager.h"
#include "game_progress.h"
#include "save_data_store.h"
#include <array>

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
		bool playTrack(MIX_Track*, MIX_Audio*, int);
		GameSceneManager::SceneCollection createScenes();
		void initializeLevel();
		void initializePlayer();
		bool persistenceReady = false;
		void cambiaEscena(Escena);
		SDL_Renderer* renderer = nullptr;
		MIX_Mixer* audioMixer = nullptr;
		MIX_Track* musicTrack = nullptr;
		static constexpr std::size_t soundTrackCount = 16;
		std::array<MIX_Track*, soundTrackCount> soundTracks{};
		std::size_t nextSoundTrack = 0;
		bool mixerInitialized = false;
		GameProgress progress;
		SaveDataStore saveDataStore;
		Camera camera;
		Player player;
		SDL_Window* window = nullptr;
		const bool* keyboard = nullptr;
		GameAssets assets;
		GameSceneManager sceneManager;
};