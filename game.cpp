
#pragma once
#include "game.h"
#include "general.h"
#include "color.h"
#include <fstream>
#include <filesystem>
Game::Game() {
	INIT_R;
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow(
		"Escape from Chickens",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_W, WINDOW_H,
		SDL_WINDOW_HIDDEN
	);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) return;
	if (Mix_Init(MIX_INIT_OGG) != MIX_INIT_OGG) return;
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) return;
	keyboard = SDL_GetKeyboardState(NULL);
	SDL_RenderSetScale(renderer, 1, 1);
	//for (const auto& entry : std::filesystem::directory_iterator("./sprites")) std::cout << entry.path() << std::endl;
	isOpen = load();
	things();
	SDL_ShowWindow(window);
	//SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
}

void Game::things() {
	camera.img = images.get("mapa3");
	botonSonido.img = images.get("soundOn");
	botonSonido.dstRect = new SDL_Rect({ 20, 20, 100, 100 });
	botonPlay.img = images.get("play");
	botonPlay.dstRect = new SDL_Rect({ 10, 135, 100, 100 });
	botonShop.img = images.get("tienda");
	botonPlay.dstRect = new SDL_Rect({ 10, 135, 100, 100 });
	nivel.dstRect = new SDL_Rect({ 0, 0, WINDOW_W, 0 });
	SDL_QueryTexture(images.get("mapa3"), NULL, NULL, NULL, &nivel.dstRect->h);
	player.dstRect = new SDL_Rect({ (WINDOW_W / 2) - 42, WINDOW_H - 200 , 50, 50 });
	player.init(images.get("link"));
	for (int i = 0; i < 3; i++) {
		player.corazones[i].img = images.get("corazon");
		player.corazones[i].alive = player.corazones[i].img;
		player.corazones[i].dead = images.get("corazont");
	}
	camera.srcRect = new SDL_Rect({ 0, nivel.dstRect->h - WINDOW_H, WINDOW_W, WINDOW_H });

	

}

bool Game::load() {
	//sfxs
	if (!tracks.load("main_theme", "sample_ogg.ogg")) return false;
	if (!images.load("soundOn", "soundOn.png")) return false;
	if (!images.load("soundOff", "soundOff.png")) return false;
	if (!images.load("mapa3", "mapa3.png")) return false;
	if (!images.load("play", "play.png")) return false;
	if (!images.load("link", "link.png")) return false;
	if (!images.load("rupia", "rupia.png")) return false;
	if (!images.load("pajaro", "pajaro.png")) return false;
	if (!images.load("corazon", "corazon.png")) return false;
	if (!images.load("corazont", "corazont.png")) return false;
	if (!images.load("enter", "press_enter.png")) return false;
	if (!images.load("pause", "pause.png")) return false;
	if (!images.load("studio", "studio.png")) return false;
	if (!images.load("horda", "horda.png")) return false;
	if (!images.load("flecha", "flecha.png")) return false;
	if (!images.load("start", "start.png")) return false;
	if (!images.load("tienda", "tienda.png")) return false;
	if (!images.load("roca1", "roca1.png")) return false;
	if (!images.load("roca2", "roca2.png")) return false;
	if (!images.load("roca3", "roca3.png")) return false;
	if (!images.load("roca4", "roca4.png")) return false;
	if (!images.load("arbol1", "arbol1.png")) return false;
	if (!images.load("arbol2", "arbol2.png")) return false;
	if (!images.load("arbol3", "arbol3.png")) return false;
	if (!images.load("arbol4", "arbol4.png")) return false;
	if (!images.load("pausaT", "pausaT.png")) return false;
	if (!images.load("gallinaGolden", "gallinaGolden.png")) return false;
	if (!images.load("gallinaMarron", "gallinaMarron.png")) return false;
	if (!images.load("gallinaBlanca", "gallinaBlanca.png")) return false;
	if (!images.load("gallinaAzul", "gallinaAzul.png")) return false;
	if (!images.load("gallinaOscura", "gallinaOscura.png")) return false;
	//Mix_PlayMusic(tracks.get("main_theme"), -1);
	return true;
}

Game::~Game(){
	images.clear();
	tracks.clear();
	sfxs.clear();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::input() {
	while (SDL_PollEvent(&event) > 0) {
		switch (event.type) {
			case SDL_QUIT:
				isOpen = false;
				break;
			case SDL_KEYDOWN:
				if (!event.key.repeat) {
					if (event.key.keysym.sym == SDLK_RETURN && escena == MENU) cambiaEscena(JOC);
					if (event.key.keysym.sym == SDLK_RETURN && escena == INICI) cambiaEscena(MENU);
					if (event.key.keysym.sym == SDLK_t && escena == MENU) cambiaEscena(TIENDA);
					if (event.key.keysym.sym == SDLK_h) player.daño();
					if (event.key.keysym.sym == SDLK_F1) god = !god;
					if (event.key.keysym.sym == SDLK_m) {
						muted = !muted;
						botonSonido.img = images.get(muted ? "soundOff" : "soundOn");
						if(!paused) muted ? Mix_PauseMusic() : Mix_ResumeMusic();
					}
					if (event.key.keysym.sym == SDLK_p) {
						if (escena == JOC || escena == PAUSA) {
							paused = !paused;
							cambiaEscena(paused ? PAUSA : JOC);
							botonPlay.img = images.get(paused ? "pause" : "play");
							if (!muted) paused ? Mix_PauseMusic() : Mix_ResumeMusic();
						}
					}
				}
				break;
			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_ENTER) {
					paused = false;
					//SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
				}
				if (event.window.event == SDL_WINDOWEVENT_LEAVE) paused = true;
				break;
		}
	}
}

void Game::cambiaEscena(Escena nuevaEscena) {
	switch (escena) {
		case INICI:
			break;
		case MENU:
			break;
		case LORE:
			break;
		case JOC:
			break;
		case GAMEOVER:
			break;
		case GUANYAT:
			break;
		case TIENDA:
			break;
		case PAUSA:
			break;
		default:
			break;
	}
	escena = nuevaEscena;
	switch (nuevaEscena) {
		case INICI:
			break;
		case MENU:
			break;
		case LORE:
			break;
		case JOC:
			break;
		case GAMEOVER:
			break;
		case GUANYAT:
			break;
		case TIENDA:
			break;
		case PAUSA:
			break;
		default:
			break;
	}
}

void Game::update() {
	if (keyboard[SDL_SCANCODE_ESCAPE]) isOpen = false;
	switch (escena) {
		case INICI:
			break;
		case MENU:
			break;
		case LORE:
			break;
		case JOC:
			if (player.vides == 0) cambiaEscena(GAMEOVER);
			if(!paused){
				if (keyboard[SDL_SCANCODE_W]) {
					player.direccion = 1;
					player.update(0, -1);
				}
				if (keyboard[SDL_SCANCODE_S]) {
					player.direccion = 3;
					player.update(0, 1);
				}
				if (keyboard[SDL_SCANCODE_A]) {
					player.direccion = 0;
					player.update(-1, 0);
				}
				if (keyboard[SDL_SCANCODE_D]) {
					player.direccion = 2;
					player.update(1, 0);
				}
				if(camera.srcRect->y > 0) camera.update();
				else {
					std::cout << "FI DEL NIVELL!!" << std::endl;
				}
			}
			break;
		case GAMEOVER:
			break;
		case GUANYAT:
			break;
		case TIENDA:
			break;
		case PAUSA:
			break;
	}
}

void Game::draw() {
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	int w, h;
	switch (escena) {
		case INICI:
			SDL_QueryTexture(images.get("studio"), NULL, NULL, &w, &h);
			SDL_RenderCopy(renderer, images.get("studio"), NULL, new SDL_Rect({(WINDOW_W / 2) - 220, 150, 440, 440}));
			SDL_QueryTexture(images.get("enter"), NULL, NULL, &w, &h);
			SDL_RenderCopy(renderer, images.get("enter"), NULL, new SDL_Rect({ WINDOW_W - 55 - w / 2, 550, w / 2, h * 7/10}));
			break;
		case MENU:
			camera.sY = 0;
			camera.draw();
			player.draw();
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
			SDL_RenderFillRect(renderer, new SDL_Rect({ 0, 0, WINDOW_W, WINDOW_H }));
			botonSonido.draw();
			botonShop.draw();
			SDL_QueryTexture(images.get("start"), NULL, NULL, &w, &h);
			SDL_RenderCopy(renderer, images.get("start"), NULL, new SDL_Rect({ WINDOW_W - 100 - w / 3, 550, w *1/3, h * 4 / 10 }));
			break;
		case LORE:
			break;
		case JOC:
			camera.sY = 2;
			camera.draw();
			botonSonido.draw();
			botonPlay.draw();
			if (SDL_GetTicks() % 20 == 0 && !paused) player.animateX();
			if (!paused) player.animateY();
			SDL_QueryTexture(images.get("horda"), NULL, NULL, &w, &h);
			SDL_RenderCopy(renderer, images.get("horda"), NULL, new SDL_Rect({0, WINDOW_H - h, w, h}));
			player.draw();
			for (int i = 0; i < 3; i++) player.corazones[i].draw();
			break;
		case GAMEOVER:
			camera.sY = 0;
			camera.draw();
			player.draw();
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
			SDL_RenderFillRect(renderer, new SDL_Rect({ 0, 0, WINDOW_W, WINDOW_H }));
			break;
		case GUANYAT:
			break;
		case TIENDA:
			camera.sY = 0;
			camera.draw();
			player.draw();
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
			SDL_RenderFillRect(renderer, new SDL_Rect({ 0, 0, WINDOW_W, WINDOW_H }));
			botonSonido.draw();
			break;
		case PAUSA:
			camera.draw();
			player.draw();
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
			SDL_RenderFillRect(renderer, new SDL_Rect({ 0, 0, WINDOW_W, WINDOW_H }));
			botonSonido.draw();
			botonPlay.draw();
			for (int i = 0; i < 3; i++) player.corazones[i].draw();
			if ((SDL_GetTicks() / 16) % 40 == 0) showPausaText = !showPausaText;
			if (showPausaText) {
				SDL_QueryTexture(images.get("pausaT"), NULL, NULL, &w, &h);
				SDL_RenderCopy(renderer, images.get("pausaT"), NULL, new SDL_Rect({ (WINDOW_W / 2) - 320 , WINDOW_H / 2 - h * 2 / 10, w * 1 / 3, h * 4 / 10 }));
			}
			break;
	}
	SDL_RenderPresent(renderer);
}

void Game::loop() {
	input();
	update();
	draw();
	SDL_Delay(1000 / 60);
}