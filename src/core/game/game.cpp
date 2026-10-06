
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
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		return;
	}
	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
		return;
	}
	window = SDL_CreateWindow(
		"Cock Flock",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_W, WINDOW_H,
		SDL_WINDOW_HIDDEN
	);
	if (window == nullptr) {
		return;
	}
	if (SDL_Surface* icon = IMG_Load(assetPath("images/icon.png").c_str()); icon != nullptr) {
		SDL_SetWindowIcon(window, icon);
		SDL_FreeSurface(icon);
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr) {
		return;
	}
	if (Mix_Init(MIX_INIT_OGG) != MIX_INIT_OGG) {
		return;
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
		return;
	}
	keyboard = SDL_GetKeyboardState(NULL);
	if (keyboard == nullptr) {
		return;
	}
	SDL_RenderSetScale(renderer, 1, 1);
	if (!assets.load(renderer)) {
		return;
	}
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
	SDL_QueryTexture(assets.images.get("mapa3"), nullptr, nullptr, nullptr, &levelHeight);
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
	assets.clear();
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
	while (SDL_PollEvent(&event) != 0) {
		switch (event.type) {
			case SDL_QUIT:
				isOpen = false;
				break;
			case SDL_KEYDOWN:
				if (!event.key.repeat) {
					sceneManager.handleInput(event);
					if (event.key.keysym.sym == SDLK_F1) {
						debugHitboxes = !debugHitboxes;
					}
					if (event.key.keysym.sym == SDLK_F4) {
						if (sceneManager.currentScene() == MENU) {
							if (debugHitboxes) cambiaEscena(GRAPHICS_ROOM);
						} else if (sceneManager.currentScene() == GRAPHICS_ROOM) {
							cambiaEscena(MENU);
						}
					}
					if (event.key.keysym.sym == SDLK_F2 && sceneManager.currentScene() == MENU) {
						sceneManager.hardModeState() = !sceneManager.hardModeState();
					}
					if (event.key.keysym.sym == SDLK_m) {
						mute();
					}
					if (event.key.keysym.sym == SDLK_p) {
						pause();
					}
				}
				break;
			case SDL_MOUSEBUTTONUP:
				sceneManager.handleClick({ event.button.x, event.button.y });
				break;
			case SDL_TEXTINPUT:
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
			pausing ? Mix_PauseMusic() : Mix_ResumeMusic();
		}
	}
}

void Game::mute() {
	muted = !muted;
	if (muted) {
		Mix_PauseMusic();
	} else if (sceneManager.currentScene() != PAUSA) {
		Mix_ResumeMusic();
	}
}

void Game::playSound(const std::string& sound, int loops) {
	if (!muted) {
		Mix_PlayChannel(-1, assets.sfxs.get(sound), loops);
	}
}

void Game::playMusic(const std::string& track, int loops) {
	Mix_PlayMusic(assets.tracks.get(track), loops);
}

void Game::haltMusic() {
	while (Mix_PlayingMusic()) {
		Mix_HaltMusic();
	}
}

void Game::haltChannels() {
	Mix_HaltChannel(-1);
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
			assets, renderer,
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
	const Uint32 frameStart = SDL_GetTicks();
	input();
	update();
	draw();
	constexpr Uint32 frameDurationMs = 1000 / 60;
	if (const Uint32 elapsed = SDL_GetTicks() - frameStart; elapsed < frameDurationMs) {
		SDL_Delay(frameDurationMs - elapsed);
	}
}