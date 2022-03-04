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
	keyboard = SDL_GetKeyboardState(NULL);
	isOpen = true;
}

Window::~Window(){
	bullets.clear();
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
				if (
					!event.key.repeat && 
					event.key.keysym.sym == SDLK_SPACE //&&
	//				!(keyboard[SDL_SCANCODE_A] || keyboard[SDL_SCANCODE_LEFT]) &&
	//				!(keyboard[SDL_SCANCODE_W] || keyboard[SDL_SCANCODE_UP]) &&
	//				!(keyboard[SDL_SCANCODE_D] || keyboard[SDL_SCANCODE_RIGHT]) &&
	//				!(keyboard[SDL_SCANCODE_S] || keyboard[SDL_SCANCODE_DOWN])
				) bullets.push_back(Bala::Bala(player.rect));
				//bullets.push_back(Cuadrado::Cuadrado(R_NUM(1, 255), R_NUM(1, 255), R_NUM(1, 255), player.facing, player.rect));
				break;
			default:
				break;
		}
	}
}

void Window::update() {
	if (keyboard[SDL_SCANCODE_ESCAPE]) isOpen = false;
	if (keyboard[SDL_SCANCODE_A] || keyboard[SDL_SCANCODE_LEFT]) {
		player.sX = -10;
		player.sY = 0;
		if (player.rect.x >= 0) player.update();
	}
	if (keyboard[SDL_SCANCODE_W] || keyboard[SDL_SCANCODE_UP]) {
		player.sX = 0;
		player.sY = -10;
		player.update();
	}
	if (keyboard[SDL_SCANCODE_D] || keyboard[SDL_SCANCODE_RIGHT]) {
		player.sX = 10;
		player.sY = 0;
		if(player.rect.x + player.rect.w < WINDOW_W) player.update();
	}
	if (keyboard[SDL_SCANCODE_S] || keyboard[SDL_SCANCODE_DOWN]) {
		player.sX = 0;
		player.sY = 10;
		player.update();
	}
	for (int i = 0; i < bullets.size(); ++i) bullets[i].update();
}

void Window::draw() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 0);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, player.cR, player.cG, player.cB, player.cA);
	SDL_RenderFillRect(renderer, &player.rect);
	for (int i = 0; i < bullets.size(); ++i) bullets[i].draw(renderer);
	SDL_RenderPresent(renderer);
	destroy();
}

void Window::destroy() {
	bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](const Cuadrado& o) { return o.disposable; }), bullets.end());
}

void Window::loop() {
	input();
	update();
	draw();
	SDL_Delay(1000 / 60);
}