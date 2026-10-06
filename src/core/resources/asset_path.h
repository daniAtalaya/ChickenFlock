#pragma once
#include <SDL3/SDL.h>
#include <stdexcept>
#include <string>

inline std::string assetPath(const std::string& relativePath) {
	const char* basePath = SDL_GetBasePath();
	if (!basePath) {
		throw std::runtime_error(std::string("Unable to locate game executable: ") + SDL_GetError());
	}
	return std::string(basePath) + "assets/" + relativePath;
}