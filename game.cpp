
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
		"Link la lia",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_W, WINDOW_H,
		SDL_WINDOW_HIDDEN
	);
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) return;
	if (Mix_Init(MIX_INIT_OGG) != MIX_INIT_OGG) return;
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) return;
	keyboard = SDL_GetKeyboardState(NULL);
	SDL_RenderSetScale(renderer, 2, 2);
	//for (const auto& entry : std::filesystem::directory_iterator("./sprites")) std::cout << entry.path() << std::endl;
	isOpen = load();
	things();
	SDL_ShowWindow(window);
	//SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
}

void Game::things() {
	player.color = Color(255, 0, 255, 0);
	camera.img = images.get("mapa2");
	botonSonido.img = images.get("soundOn");
	botonSonido.dstRect = new SDL_Rect({ 50, 50, 50, 50 });
	player.srcRect = NULL;
	nivel.dstRect = new SDL_Rect({ 0, 0, 0, 0 });
	SDL_QueryTexture(images.get("mapa2"), NULL, NULL, &nivel.dstRect->w, &nivel.dstRect->h);
	player.dstRect = new SDL_Rect({ 50, 50 , 50, 50 });
	camera.dstRect = new SDL_Rect({ 0, 0, WINDOW_W, WINDOW_H });
	camera.srcRect = new SDL_Rect({ 0, nivel.dstRect->h - WINDOW_H, WINDOW_W, WINDOW_H });
}

bool Game::load() {
	if (!tracks.load("main_theme", "sample_ogg.ogg")) return false;
	if (!images.load("soundOn", "soundOn.png")) return false;
	if (!images.load("soundOff", "soundOff.png")) return false;
	if (!images.load("mapa2", "mapa2.png")) return false;
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
					if (event.key.keysym.sym == SDLK_F1) god = !god;
					if (event.key.keysym.sym == SDLK_m) {
						muted = !muted;
						botonSonido.img = images.get(muted ? "soundOff" : "soundOn");
						if(!paused) muted ? Mix_PauseMusic() : Mix_ResumeMusic();
					}
					if (event.key.keysym.sym == SDLK_p) {
						paused = !paused;
						if (!muted) paused ? Mix_PauseMusic() : Mix_ResumeMusic();
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
		case JOC:
			break;
		case GAMEOVER:
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
		case JOC:
			break;
		case GAMEOVER:
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
			if (keyboard[SDL_SCANCODE_RETURN]) escena = MENU;
			break;
		case MENU:
			break;
		case JOC:
			if (keyboard[SDL_SCANCODE_W]) {
				player.update(0, -1);
			}
			if (keyboard[SDL_SCANCODE_S]) {
				player.update(0, 1);
			}
			if (keyboard[SDL_SCANCODE_A]) {
				player.update(-1, 0);
			}
			if (keyboard[SDL_SCANCODE_D]) {
				player.update(1, 0);
			}
			camera.update();
			break;
		case GAMEOVER:
			break;
		case TIENDA:
			break;
		case PAUSA:
			break;
	}
}

void Game::draw() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	switch (escena) {
		case INICI:
			break;
		case MENU:
			break;
		case JOC:
			camera.draw();
			botonSonido.draw();
			player.draw();
			break;
		case GAMEOVER:
			break;
		case TIENDA:
			break;
		case PAUSA:
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