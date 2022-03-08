
#pragma once
#include "window.h"
#include "bala.h"
#include "general.h"

Window::Window() {
	INIT_R;
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow(
		WINDOW_T,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_W, WINDOW_H,
		SDL_WINDOW_SHOWN
	);
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) return;
	img_background = SDL_CreateTextureFromSurface(renderer, IMG_Load("background.png"));
	if (img_background == NULL) return;
	img_player = SDL_CreateTextureFromSurface(renderer, IMG_Load("spaceship.png"));
	if (img_player == NULL) return;
	img_shot = SDL_CreateTextureFromSurface(renderer, IMG_Load("shot.png"));
	if (img_shot == NULL) return;
	keyboard = SDL_GetKeyboardState(NULL);
	player.rect = { 20, WINDOW_H >> 1, 104, 82 };
	int w;
	SDL_QueryTexture(img_background, NULL, NULL, &w, NULL);
	fondo.rect = { 0, 0, w, WINDOW_H };
	fondo.rect.w = w;
	fondo.sX = fondo.sY = 4;
	isOpen = true;
}

Window::~Window(){
	bullets.clear();
	SDL_DestroyTexture(img_background);
	SDL_DestroyTexture(img_player);
	SDL_DestroyTexture(img_shot);
	IMG_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Window::input() {
	if (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				isOpen = false;
				break;
			case SDL_KEYDOWN:
				if (!event.key.repeat) {
					if (event.key.keysym.sym == SDLK_F1) godMode = !godMode;
					if (event.key.keysym.sym == SDLK_SPACE) bullets.push_back(Bala::Bala(player.rect));
				}
				break;
		}
	}
}

void Window::update() {
	if (keyboard[SDL_SCANCODE_ESCAPE]) isOpen = false;
	if (keyboard[SDL_SCANCODE_A] || keyboard[SDL_SCANCODE_LEFT]) if (player.rect.x >= 0) player.update(-1, 0);
	if (keyboard[SDL_SCANCODE_W] || keyboard[SDL_SCANCODE_UP]) player.update(0, -1);
	if (keyboard[SDL_SCANCODE_D] || keyboard[SDL_SCANCODE_RIGHT]) if(player.rect.x + player.rect.w < WINDOW_W) player.update(1, 0);
	if (keyboard[SDL_SCANCODE_S] || keyboard[SDL_SCANCODE_DOWN]) player.update(0, 1);
	for (int i = 0; i < bullets.size(); ++i) bullets[i].update(1, 1);
	destroyBullets();
	fondo.update(-1, 0);
	if (fondo.rect.x <= -fondo.rect.w + WINDOW_W) fondo.rect.x = 0;
	//std::cout << "Nº Balas: " << bullets.size() << std::endl;
}

void Window::draw() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 0);
	SDL_RenderClear(renderer);
	if (godMode) SDL_SetRenderDrawColor(renderer, 192, 0, 0, 255);
	SDL_RenderCopy(renderer, img_background, NULL, &fondo.rect);
	SDL_RenderCopy(renderer, img_player, NULL, &player.rect);
	if (godMode) SDL_RenderDrawRect(renderer, &player.rect);
	for (int i = 0; i < bullets.size(); ++i) bullets[i].draw(renderer, img_shot, godMode);
	SDL_RenderPresent(renderer);
}

void Window::destroyBullets() {
	bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](const Cuadrado& o) { 
		return o.rect.x < -o.rect.w || o.rect.x > WINDOW_W + o.rect.w || o.rect.y < -o.rect.h || o.rect.y > WINDOW_H + o.rect.h; 
	}), bullets.end());
}

void Window::loop() {
	input();
	update();
	draw();
	SDL_Delay(1000 / 60);
}