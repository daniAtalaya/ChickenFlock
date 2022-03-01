#include "window.h"
#include "general.h"

Window::Window() {
	SDL_Init(SDL_INIT_EVERYTHING);
	_window = SDL_CreateWindow(
		WINDOW_T,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_W, WINDOW_H,
		SDL_WINDOW_SHOWN
	);
	_renderer = SDL_CreateRenderer(_window, -1, 0);
	_keyboard = SDL_GetKeyboardState(NULL);
	isOpen = true;
}

Window::~Window(){
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
	SDL_Quit();
}

void Window::input() {
	if (SDL_PollEvent(&_event)) {
		switch (_event.type) {
			case SDL_QUIT:
				isOpen = false;
				break;
			default:
				break;
		}
	}
}

void Window::update() {
	if (_keyboard[SDL_SCANCODE_ESCAPE]) isOpen = false;
	if (_keyboard[SDL_SCANCODE_A] || _keyboard[SDL_SCANCODE_LEFT]) player.move(0, 0);
	if (_keyboard[SDL_SCANCODE_W] || _keyboard[SDL_SCANCODE_UP]) player.move(1, 0);
	if (_keyboard[SDL_SCANCODE_D] || _keyboard[SDL_SCANCODE_RIGHT]) player.move(0, 1);
	if (_keyboard[SDL_SCANCODE_S] || _keyboard[SDL_SCANCODE_DOWN]) player.move(1, 1);
}

void Window::draw() {
	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0);
	SDL_RenderClear(_renderer);
	SDL_SetRenderDrawColor(_renderer, player.cR, player.cG, player.cB, player.cA);
	SDL_RenderFillRect(_renderer, &player.rect);
	SDL_RenderPresent(_renderer);
}
