
#pragma once
#include "game.h"
#include "general.h"

Game::Game() {
	INIT_R;
	SDL_Init(SDL_INIT_EVERYTHING);
	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) return;
	if (Mix_Init(MIX_INIT_OGG) != MIX_INIT_OGG) return;
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) return;
	if (!tracks.loadMusic("main_theme", "sample_ogg.ogg")) return;
	if (!sfxs.loadEffect("bruh", "sample_wav.wav")) return;
	window = SDL_CreateWindow(
		WINDOW_T,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_W, WINDOW_H,
		SDL_WINDOW_SHOWN
	);
	renderer = SDL_CreateRenderer(window, -1, 0);
	keyboard = SDL_GetKeyboardState(NULL);
	fondo.rect = { 0, 0, WINDOW_W, WINDOW_H };
	Mix_PlayMusic(tracks.get("main_theme"), -1);
	isOpen = true;
}

Game::~Game(){
	for (std::map<std::string, SDL_Texture*>::iterator iterador = images.mapa.begin(); iterador != images.mapa.end(); iterador++) {
		SDL_DestroyTexture(iterador->second);
	}
	for (std::map<std::string, Mix_Music*>::iterator iterador = tracks.mapa.begin(); iterador != tracks.mapa.end(); iterador++) {
		Mix_FreeMusic(iterador->second);
	}
	for (std::map<std::string, Mix_Chunk*>::iterator iterador = sfxs.mapa.begin(); iterador != sfxs.mapa.end(); iterador++) {
		Mix_FreeChunk(iterador->second);
	}
	IMG_Quit();
	Mix_CloseAudio();
	while (Mix_Init(0)) Mix_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::input() {
	if (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				isOpen = false;
				break;
			case SDL_KEYDOWN:
				if (!event.key.repeat) {
					if (event.key.keysym.sym == SDLK_F1) god = !god;
					if (event.key.keysym.sym == SDLK_SPACE) {
						paused = !paused;
						paused ? Mix_PauseMusic() : Mix_ResumeMusic();
						fondo.cB = paused ? 128 : 0; 
						fondo.cR = paused ? 128 : 0;
						//Mix_PlayChannel(-1, sfxs.get("bruh"), 0);
					}
				}
				break;
		}
	}
}

void Game::update() {
	if (keyboard[SDL_SCANCODE_ESCAPE]) isOpen = false;
	//if (keyboard[SDL_SCANCODE_A] || keyboard[SDL_SCANCODE_LEFT]) if (player.rect.x >= 0) player.update(-1, 0);
	//if (keyboard[SDL_SCANCODE_W] || keyboard[SDL_SCANCODE_UP]) player.update(0, -1);
	//if (keyboard[SDL_SCANCODE_D] || keyboard[SDL_SCANCODE_RIGHT]) if(player.rect.x + player.rect.w < WINDOW_W) player.update(1, 0);
	//if (keyboard[SDL_SCANCODE_S] || keyboard[SDL_SCANCODE_DOWN]) player.update(0, 1);
}

void Game::draw() {
	SDL_SetRenderDrawColor(renderer, fondo.cR, fondo.cG, fondo.cB, fondo.cA);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
}

/*void Window::outsideScren() {
	bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](const Cuadrado& o) { 
		return o.rect.x < -o.rect.w || o.rect.x > WINDOW_W + o.rect.w || o.rect.y < -o.rect.h || o.rect.y > WINDOW_H + o.rect.h;
	}), bullets.end());
}*/

void Game::loop() {
	input();
	update();
	draw();
	SDL_Delay(1000 / 60);
}