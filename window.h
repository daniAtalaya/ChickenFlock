#pragma once

#include <string>
#include "SDL/include/SDL.h"
#pragma comment( lib, "SDL/x64/SDL2.lib" )
#pragma comment( lib, "SDL/x64/SDL2main.lib" )

class Window {
	public:
		Window(const std::string& title);
		~Window();
		void update();
		void draw();
		static const int width = 800;
		static const int height = 600;
		SDL_Rect rect;
		inline bool isClosed() const { return _closed; }
	private:
		bool init();
		std::string _title;
		bool _closed = false;
		SDL_Window* _window = nullptr;
		SDL_Renderer* _renderer = nullptr;

};