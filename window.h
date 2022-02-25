#pragma once

#include "general.h"
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