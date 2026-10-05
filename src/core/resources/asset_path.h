#pragma once
#include <SDL.h>
#include <memory>
#include <stdexcept>
#include <string>

struct SdlPathDeleter {
	void operator()(char* path) const {
		SDL_free(path);
	}
};

inline std::string assetPath(const std::string& relativePath) {
	std::unique_ptr<char, SdlPathDeleter> basePath(SDL_GetBasePath());
	if (!basePath) {
		throw std::runtime_error(std::string("Unable to locate game executable: ") + SDL_GetError());
	}
	return std::string(basePath.get()) + "assets/" + relativePath;
}