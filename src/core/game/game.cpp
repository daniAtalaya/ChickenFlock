
#include "game.h"
#include "game_renderer.h"
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
	assignImg();
	init();
	const auto context = sceneFactoryContext();
	sceneManager.initialize(escena, context);
	isOpen = true;
	SDL_ShowWindow(window);
	SDL_GetMouseState(&mouse->x, &mouse->y);
}

void Game::assignImg() {
	horda.img = assets.images.get("horda");
	camera.img = assets.images.get("mapa3");
	botonBack.img = assets.images.get("back");
	botonShop.img = assets.images.get("tienda");
	botonHardcore.img = assets.images.get("hardcore");
	creditos.img = assets.images.get("creditos");
}

void Game::init() {
	botonPlay.img = assets.images.get("play");
	assignRect(paredHitboxLeft.dstRect, { 1, 1, 150, 8100 });
	assignRect(paredHitboxRight.dstRect, { 810, 1, 150, 8100 });
	botonSonido.img = assets.images.get("soundOn");
	assignRect(botonSonido.dstRect, { 10, 20, 100, 100 });
	assignRect(botonPlay.dstRect, { 10, 135, 100, 100 });
	assignRect(botonBack.dstRect, { 10, 250, 100, 100 });
	assignRect(botonShop.dstRect, { 10, 135, 100, 100 });
	assignRect(botonHardcore.dstRect, { (WINDOW_W / 2) - 410, 750, 330, 100 });
	assignRect(creditos.dstRect, { 75, (WINDOW_H * 15 / 10), WINDOW_W - 150, WINDOW_H * 16 / 10 });
	creditos.sY = 2;
	assignRect(nivel.dstRect, { 0, 0, WINDOW_W, 0 });
	assignRect(player.dstRect, { (WINDOW_W / 2) - 42, WINDOW_H - 300 , 50, 50 });
	SDL_QueryTexture(assets.images.get("mapa3"), NULL, NULL, NULL, &nivel.dstRect->h);
	assignRect(camera.dstRect, { 0, 0, WINDOW_W, WINDOW_H });
	assignRect(camera.srcRect, { 0, nivel.dstRect->h - WINDOW_H, WINDOW_W, WINDOW_H });
	assignRect(horda.dstRect, { 130, WINDOW_H, 0, 0 });
	continuara.img = assets.images.get("continuara");
	continuara.sY = 6;
	pajaro.init(assets.images.get("pajaro"));
	assignRect(pajaro.dstRect, { -200, R_NUM(0, WINDOW_H - 200), 70, 100 });
	pajaro.sX = 8;
	pajaro.sY = R_NUM(-4, 4);
	avestruz.sY = 1;
	assignRect(continuara.dstRect, { WINDOW_H / 2 - 325, (WINDOW_H * 15 / 10), 700 , 450 });
	avestruz.init(assets.images.get("avestruz"));
	mascota.init(assets.images.get("mascota"));
	assignRect(mascota.dstRect, { (WINDOW_W / 2) + 150, 75, 200, 200 });
	assignRect(avestruz.dstRect, { WINDOW_H / 2 - 100, (WINDOW_H * 15 / 10), 200, 200 });
	player.init(assets.images.get("link"));
	SDL_QueryTexture(assets.images.get("horda"), nullptr, nullptr, &horda.dstRect->w, &horda.dstRect->h);
	horda.dstRect->y = WINDOW_H - horda.dstRect->h;
	player.vides = 3;
	for (int i = 0; i < 3; i++) {
		player.corazones[i].img = assets.images.get("corazon");
		player.corazones[i].alive = player.corazones[i].img;
		player.corazones[i].dead = assets.images.get("corazont");
		player.corazones[i].img = player.corazones[i].alive;
	}
}

Game::~Game() {
	world.clear();
	assets.clear();
	if (renderer != nullptr) {
		SDL_DestroyRenderer(renderer);
	}
	if (window != nullptr) {
		SDL_DestroyWindow(window);
	}
	delete mouse;
	SDL_Quit();
}

void Game::input() {
	while (SDL_PollEvent(&event) != 0) {
		switch (event.type) {
			case SDL_QUIT:
				isOpen = false;
				break;
			case SDL_KEYDOWN:
				if (!event.key.repeat) {
					sceneManager.handleInput(event);
					if (event.key.keysym.sym == SDLK_F1) {
						god = !god;
					}
					if (event.key.keysym.sym == SDLK_m) {
						mute();
					}
					if (event.key.keysym.sym == SDLK_p) {
						pause();
					}
				}
				break;
			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_ENTER) {
					paused = false;
				}
				if (event.window.event == SDL_WINDOWEVENT_LEAVE) {
					paused = true;
				}
				break;
			case SDL_MOUSEMOTION:
				SDL_GetMouseState(&mouse->x, &mouse->y);
				break;
			case SDL_MOUSEBUTTONUP:
				isClicking = true;
				break;
			default:
				break;
		}
	}
}

void Game::pause() {
	if (escena == JOC || escena == PAUSA) {
		paused = !paused;
		cambiaEscena(paused ? PAUSA : JOC);
		botonPlay.img = assets.images.get(paused ? "pause" : "play");
		if (!muted) {
			paused ? Mix_PauseMusic() : Mix_ResumeMusic();
		}
	}
}

void Game::mute() {
	muted = !muted;
	botonSonido.img = assets.images.get(muted ? "soundOff" : "soundOn");
	muted ? Mix_PauseMusic() : Mix_ResumeMusic();
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

GameSceneFactoryContext Game::sceneFactoryContext() {
	return {
		renderer,
		escena,
		assets,
		world,
		player,
		camera,
		pajaro,
		mascota,
		avestruz,
		horda,
		paredHitboxLeft,
		paredHitboxRight,
		creditos,
		continuara,
		botonSonido,
		botonPlay,
		botonShop,
		botonBack,
		botonHardcore,
		god,
		muted,
		paused,
		hardMode,
		isClicking,
		mouse,
		keyboard,
		partidesJugades,
		dineroTemporal,
		[this](const Escena scene) { cambiaEscena(scene); },
		[this] { init(); },
		[this] { mute(); },
		[this] { pause(); },
		[this](const std::string& sound, int loops) { playSound(sound, loops); },
		[this](const std::string& track, int loops) { playMusic(track, loops); },
		[this]{ haltMusic(); },
		[this]{ haltChannels(); }
	};
}

void Game::cambiaEscena(Escena nuevaEscena) {
	sceneManager.changeTo(nuevaEscena);
}

void Game::update() {
	world.cleanup();
	if (keyboard[SDL_SCANCODE_ESCAPE]) {
		isOpen = false;
	}
	if (isClicking) {
		sceneManager.handleClick();
	}
	sceneManager.update();
}

void Game::draw() {
	GameRenderer::beginFrame(renderer);
	sceneManager.render();
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