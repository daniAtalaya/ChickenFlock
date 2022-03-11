
#pragma once
#include "game.h"
#include "general.h"
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
	for (const auto& entry : std::filesystem::directory_iterator("./sprites")) std::cout << entry.path() << std::endl;
	fondo.rect = { 0, 0, WINDOW_W, WINDOW_H };
	isOpen = load();
	SDL_ShowWindow(window);
	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
}

bool Game::load() {
	if (!tracks.load("main_theme", "sample_ogg.ogg")) return false;
	if (!sfxs.load("d", "sample_wav.wav")) return false;
	if (!images.load("cosa", "sprites/tile027.png")) return false;
	Mix_PlayMusic(tracks.get("main_theme"), -1);
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
					if (event.key.keysym.sym == SDLK_SPACE) paused = !paused;
				}
				break;
			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_ENTER) {
					paused = false;
					SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
				}
				if (event.window.event == SDL_WINDOWEVENT_LEAVE) paused = true;
				break;
		}
	}
}

void Game::update() {
	if (keyboard[SDL_SCANCODE_ESCAPE]) isOpen = false;
	fondo.color.r = paused ? 255 : 111;
}

void Game::draw() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	fondo.draw();
	SDL_Rect* rc = new SDL_Rect({ 100, 100, 128, 128 });
	SDL_RenderCopy(renderer, images.get("cosa"), NULL, rc);
	SDL_RenderPresent(renderer);
}

void Game::loop() {
	input();
	update();
	draw();
	SDL_Delay(1000 / 60);
}