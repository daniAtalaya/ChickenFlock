
#include "game.h"
#include "game_renderer.h"
#include "scenes/intro/intro_scene_factory.h"
#include "scenes/menu/menu_scene_factory.h"
#include "scenes/lore/lore_scene_factory.h"
#include "scenes/gameplay/gameplay_scene_factory.h"
#include "scenes/game_over/game_over_scene_factory.h"
#include "scenes/victory/victory_scene_factory.h"
#include "scenes/shop/shop_scene_factory.h"
#include "scenes/pause/pause_scene_factory.h"
#include "scenes/credits/credits_scene_factory.h"
#include "scenes/graphics_room/graphics_room_scene_factory.h"
#include "resources/asset_path.h"

namespace {
	void assignRect(SDL_Rect*& target, const SDL_Rect& value) {
		if (target == nullptr) {
			target = new SDL_Rect(value);
		} else {
			*target = value;
		}
	}

}

Game::Game() {
	INIT_R;
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		return;
	}
	window = SDL_CreateWindow("Cock Flock", WINDOW_W, WINDOW_H, SDL_WINDOW_HIDDEN);
	if (window == nullptr) {
		return;
	}
	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	if (SDL_Surface* icon = IMG_Load(assetPath("images/icon.png").c_str()); icon != nullptr) {
		SDL_SetWindowIcon(window, icon);
		SDL_DestroySurface(icon);
	}
	renderer = SDL_CreateRenderer(window, nullptr);
	if (renderer == nullptr) {
		return;
	}
	SDL_SetRenderVSync(renderer, 1);
	if (!MIX_Init()) {
		return;
	}
	mixerInitialized = true;
	audioMixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
	if (audioMixer == nullptr) {
		return;
	}
	musicTrack = MIX_CreateTrack(audioMixer);
	if (musicTrack == nullptr) return;
	for (MIX_Track*& track : soundTracks) {
		track = MIX_CreateTrack(audioMixer);
		if (track == nullptr) return;
	}
	keyboard = SDL_GetKeyboardState(NULL);
	if (keyboard == nullptr) {
		return;
	}
	SDL_SetRenderScale(renderer, 1.0f, 1.0f);
	if (!assets.load(renderer, audioMixer)) {
		return;
	}
	playMusic("Intro", 1);
	progress = saveDataStore.load();
	persistenceReady = true;
	init();
	sceneManager.initialize(INICI, createScenes(), {
		progress,
		[this] { haltMusic(); },
		[this](const GameProgress& updated) { saveDataStore.requestSave(updated); }
	});
	isOpen = true;
	SDL_ShowWindow(window);
}

void Game::init() {
	initializeLevel();
	initializePlayer();
}

void Game::initializeLevel() {
	camera.img = assets.images.get("mapa3");
	int levelHeight = 0;
	getTextureSize(assets.images.get("mapa3"), nullptr, &levelHeight);
	assignRect(camera.dstRect, { 0, 0, WINDOW_W, WINDOW_H });
	assignRect(camera.srcRect, { 0, levelHeight - WINDOW_H, WINDOW_W, WINDOW_H });
}

void Game::initializePlayer() {
	assignRect(player.dstRect, { (WINDOW_W / 2) - 42, WINDOW_H - 300, 50, 50 });
	player.init(assets.images.get("link"));
	player.vides = 3;
	for (Corazon& heart : player.corazones) {
		heart.img = assets.images.get("corazon");
		heart.alive = heart.img;
		heart.dead = assets.images.get("corazont");
	}
}

Game::~Game() {
	if (persistenceReady) {
		saveDataStore.requestSave(progress);
		saveDataStore.shutdown();
	}
	sceneManager.shutdown();
	haltMusic();
	haltChannels();
	if (musicTrack != nullptr) MIX_SetTrackAudio(musicTrack, nullptr);
	for (MIX_Track* track : soundTracks) {
		if (track != nullptr) MIX_SetTrackAudio(track, nullptr);
	}
	assets.clear();
	if (musicTrack != nullptr) MIX_DestroyTrack(musicTrack);
	for (MIX_Track* track : soundTracks) {
		if (track != nullptr) MIX_DestroyTrack(track);
	}
	if (audioMixer != nullptr) MIX_DestroyMixer(audioMixer);
	if (mixerInitialized) MIX_Quit();
	if (renderer != nullptr) {
		SDL_DestroyRenderer(renderer);
	}
	if (window != nullptr) {
		SDL_DestroyWindow(window);
	}
	SDL_Quit();
}

void Game::input() {
	SDL_Event event{};
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_EVENT_QUIT:
				isOpen = false;
				break;
			case SDL_EVENT_KEY_DOWN:
				if (!event.key.repeat) {
					sceneManager.handleInput(event);
					if (event.key.key == SDLK_F1) {
						debugHitboxes = !debugHitboxes;
					}
					if (event.key.key == SDLK_F4) {
						if (sceneManager.currentScene() == MENU) {
							if (debugHitboxes) cambiaEscena(GRAPHICS_ROOM);
						} else if (sceneManager.currentScene() == GRAPHICS_ROOM) {
							cambiaEscena(MENU);
						}
					}
					if (event.key.key == SDLK_F2 && sceneManager.currentScene() == MENU) {
						sceneManager.hardModeState() = !sceneManager.hardModeState();
					}
					if (event.key.key == SDLK_M) {
						mute();
					}
					if (event.key.key == SDLK_P) {
						pause();
					}
				}
				break;
			case SDL_EVENT_MOUSE_BUTTON_UP:
				sceneManager.handleClick({ static_cast<int>(event.button.x),
					static_cast<int>(event.button.y) });
				break;
			case SDL_EVENT_TEXT_INPUT:
				sceneManager.handleInput(event);
				break;
			default:
				break;
		}
	}
}

void Game::pause() {
	if (sceneManager.currentScene() == JOC || sceneManager.currentScene() == PAUSA) {
		const bool pausing = sceneManager.currentScene() == JOC;
		cambiaEscena(pausing ? PAUSA : JOC);
		if (!muted) {
			pausing ? MIX_PauseTrack(musicTrack) : MIX_ResumeTrack(musicTrack);
		}
	}
}

void Game::mute() {
	muted = !muted;
	if (muted) {
		MIX_PauseTrack(musicTrack);
	} else if (sceneManager.currentScene() != PAUSA) {
		MIX_ResumeTrack(musicTrack);
	}
}

void Game::playSound(const std::string& sound, int loops) {
	if (!muted) {
		MIX_Track* track = soundTracks[nextSoundTrack];
		nextSoundTrack = (nextSoundTrack + 1) % soundTracks.size();
		playTrack(track, assets.sfxs.get(sound), loops);
	}
}

void Game::playMusic(const std::string& track, int loops) {
	playTrack(musicTrack, assets.tracks.get(track), loops);
}

bool Game::playTrack(MIX_Track* track, MIX_Audio* audio, const int loops) {
	if (track == nullptr || audio == nullptr
		|| !MIX_SetTrackAudio(track, audio)) {
		SDL_LogError(SDL_LOG_CATEGORY_AUDIO, "Unable to set audio track input: %s", SDL_GetError());
		return false;
	}
	const SDL_PropertiesID options = SDL_CreateProperties();
	if (options == 0) {
		SDL_LogError(SDL_LOG_CATEGORY_AUDIO, "Unable to create playback options: %s", SDL_GetError());
		return false;
	}
	const bool configured = SDL_SetNumberProperty(options, MIX_PROP_PLAY_LOOPS_NUMBER, loops);
	const bool played = configured && MIX_PlayTrack(track, options);
	SDL_DestroyProperties(options);
	if (!played) {
		SDL_LogError(SDL_LOG_CATEGORY_AUDIO, "Unable to play audio track: %s", SDL_GetError());
	}
	return played;
}

void Game::haltMusic() {
	if (musicTrack != nullptr) MIX_StopTrack(musicTrack, 0);
}

void Game::haltChannels() {
	for (MIX_Track* track : soundTracks) {
		if (track != nullptr) MIX_StopTrack(track, 0);
	}
}

GameSceneManager::SceneCollection Game::createScenes() {
	return {
		createIntroScene({
			assets, [this](const Escena scene) { cambiaEscena(scene); },
			[this] { mute(); }, muted
		}),
		createMenuScene({
			assets, [this] { haltChannels(); },
			[this](const std::string& track, int loops) { playMusic(track, loops); },
			progress, [this] { init(); },
			[this](const Escena scene) { cambiaEscena(scene); },
			[this] { mute(); }, muted, debugHitboxes, sceneManager.hardModeState(), camera, player
		}),
		createLoreScene({
			assets, muted,
			[this](const std::string& sound, int loops) { playSound(sound, loops); },
			[this](const Escena scene) { cambiaEscena(scene); },
			[this] { mute(); }, camera, player
		}),
		createGameplayScene({
			assets, muted,
			[this](const std::string& track, int loops) { playMusic(track, loops); },
			[this](const std::string& sound, int loops) { playSound(sound, loops); },
			sceneManager.hardModeState(), player, camera, keyboard, progress,
			[this](const Escena scene) { cambiaEscena(scene); },
			[this] { mute(); }, [this] { pause(); }, debugHitboxes
		}),
		createGameOverScene({
			assets, progress, muted,
			[this](const std::string& track, int loops) { playMusic(track, loops); },
			[this] { init(); },
			[this](const Escena scene) { cambiaEscena(scene); },
			[this] { mute(); }, camera, player
		}),
		createVictoryScene({
			assets, progress, muted, [this] { haltChannels(); },
			[this](const std::string& track, int loops) { playMusic(track, loops); },
			[this] { init(); },
			[this](const Escena scene) { cambiaEscena(scene); },
			[this] { mute(); }, camera, player
		}),
		createShopScene({
			assets, muted,
			[this](const std::string& track, int loops) { playMusic(track, loops); },
			player, progress, [this](const Escena scene) { cambiaEscena(scene); },
			[this] { mute(); }, camera
		}),
		createPauseScene({
			assets, muted, [this] { haltChannels(); },
			[this](const Escena scene) { cambiaEscena(scene); },
			[this] { mute(); }, camera, player, [this] { pause(); }
		}),
		createCreditsScene({
			assets, muted,
			[this] { haltChannels(); },
			[this](const std::string& track, int loops) { playMusic(track, loops); },
			[this](const Escena scene) { cambiaEscena(scene); },
			[this] { mute(); }
		}),
		createGraphicsRoomScene({
			assets, renderer, audioMixer, window,
			[this](const Escena scene) { cambiaEscena(scene); }
		})
	};
}

void Game::cambiaEscena(const Escena nuevaEscena) {
	sceneManager.changeTo(nuevaEscena);
}

void Game::update() {
	if (keyboard[SDL_SCANCODE_ESCAPE]) {
		isOpen = false;
	}
	sceneManager.update();
	if (persistenceReady) {
		saveDataStore.requestSave(progress);
	}
}

void Game::draw() const {
	GameRenderer::beginFrame(renderer);
	sceneManager.render(renderer, debugHitboxes);
	GameRenderer::endFrame(renderer);
}

void Game::loop() {
	const Uint64 frameStart = SDL_GetTicks();
	input();
	update();
	draw();
	constexpr Uint32 frameDurationMs = 1000 / 60;
	if (const Uint64 elapsed = SDL_GetTicks() - frameStart; elapsed < frameDurationMs) {
		SDL_Delay(static_cast<Uint32>(frameDurationMs - elapsed));
	}
}