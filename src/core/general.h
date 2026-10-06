#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <string>
#include <vector>
#include <cmath>
#include <map>
#include <iostream>
#include <functional>
#include <cstdlib>
#include <ctime>

#define WINDOW_W 960
#define WINDOW_H 900
#define INIT_R std::srand(static_cast<unsigned int>(std::time(nullptr)))
#define R_NUM(min, max) min + rand() % ((max + 1) - min)

inline SDL_FRect toFloatRect(const SDL_Rect& rectangle) {
	return { static_cast<float>(rectangle.x), static_cast<float>(rectangle.y),
		static_cast<float>(rectangle.w), static_cast<float>(rectangle.h) };
}

inline void renderTexture(SDL_Renderer* renderer, SDL_Texture* texture,
	const SDL_Rect* source, const SDL_Rect* destination) {
	const SDL_FRect sourceFloat = source == nullptr ? SDL_FRect{} : toFloatRect(*source);
	const SDL_FRect destinationFloat = destination == nullptr ? SDL_FRect{} : toFloatRect(*destination);
	SDL_RenderTexture(renderer, texture, source == nullptr ? nullptr : &sourceFloat,
		destination == nullptr ? nullptr : &destinationFloat);
}

inline void renderTexture(SDL_Renderer* renderer, SDL_Texture* texture,
	const SDL_Rect& destination) {
	renderTexture(renderer, texture, nullptr, &destination);
}

inline void renderTexture(SDL_Renderer* renderer, SDL_Texture* texture,
	const SDL_Rect& source, const SDL_Rect& destination) {
	renderTexture(renderer, texture, &source, &destination);
}

inline void fillRect(SDL_Renderer* renderer, const SDL_Rect& rectangle) {
	const SDL_FRect destination = toFloatRect(rectangle);
	SDL_RenderFillRect(renderer, &destination);
}

inline void drawRect(SDL_Renderer* renderer, const SDL_Rect& rectangle) {
	const SDL_FRect destination = toFloatRect(rectangle);
	SDL_RenderRect(renderer, &destination);
}

inline bool getTextureSize(SDL_Texture* texture, int* width, int* height) {
	float textureWidth = 0.0f;
	float textureHeight = 0.0f;
	if (!SDL_GetTextureSize(texture, &textureWidth, &textureHeight)) {
		return false;
	}
	if (width != nullptr) *width = static_cast<int>(textureWidth);
	if (height != nullptr) *height = static_cast<int>(textureHeight);
	return true;
}