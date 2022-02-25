#include "window.h"
#include <iostream>

Window::Window(const std::string& title) :
_title(title) {
	_closed = !init();
}

Window::~Window(){
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
	SDL_Quit();
}

bool Window::init() {
	SDL_Init(SDL_INIT_EVERYTHING);
	_window = SDL_CreateWindow(
		_title.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width, height,
		SDL_WINDOW_SHOWN
	);
	_renderer = SDL_CreateRenderer(_window, -1, 0);
	return true;
}

void Window::draw() {
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				_closed = true;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
					case SDLK_ESCAPE:
						_closed = true;
						break;
					case SDLK_w:
						rect.y -= 5;;
						break;
					case SDLK_a:
						rect.x -= 5;
						break;
					case SDLK_s:
						rect.x += 5;
						break;
					case SDLK_d:
						rect.y += 5;
						break;
				}
			default:
				break;
		}
	}
}

void Window::update() {
	SDL_SetRenderDrawColor(_renderer, 0, 120, 200, 255);
	SDL_RenderClear(_renderer);
	SDL_SetRenderDrawColor(_renderer, 200, 0, 200, 255); 
	SDL_RenderFillRect(_renderer, &rect);
	SDL_RenderPresent(_renderer);
}