#include "window.h"
#include "general.h"


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
	rect.x = 100;
	rect.y = 100;
	rect.w = 50;
	rect.h = 75;
	return true;
}

void Window::draw() {
	SDL_SetRenderDrawColor(_renderer, 0, 120, 200, 255);
	SDL_RenderClear(_renderer);
	SDL_SetRenderDrawColor(_renderer, 200, 0, 200, 255);
	SDL_RenderFillRect(_renderer, &rect);
	SDL_RenderPresent(_renderer);
}

void Window::update() {
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		const Uint8* ks = SDL_GetKeyboardState(NULL);
		switch (event.type) {
		case SDL_QUIT:
			_closed = true;
			break;
		default:
			break;
		}
		if (ks[SDL_SCANCODE_ESCAPE])_closed = true;
		if (ks[SDL_SCANCODE_W] || ks[SDL_SCANCODE_UP]) rect.y -= 10;
		if (ks[SDL_SCANCODE_A] || ks[SDL_SCANCODE_LEFT]) rect.x -= 10;
		if (ks[SDL_SCANCODE_S] || ks[SDL_SCANCODE_DOWN]) rect.y += 10;
		if (ks[SDL_SCANCODE_D] || ks[SDL_SCANCODE_RIGHT]) rect.x += 10;
	}
}