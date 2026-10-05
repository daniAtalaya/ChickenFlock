#include "almacen.h"
#include "asset_path.h"
#include "game.h"

template <>
bool Almacen<SDL_Texture*>::load(const std::string &name, const std::string &filename) {
	SDL_Surface* surface = IMG_Load(assetPath("images/" + filename).c_str());
	if (surface == nullptr) return false;
	mapa[name] = SDL_CreateTextureFromSurface(Game::renderer, surface);
	SDL_FreeSurface(surface);
	return mapa[name] != nullptr;
}
template <>
bool Almacen<Mix_Music*>::load(const std::string &name, const std::string &filename) {
	mapa[name] = Mix_LoadMUS(assetPath("audio/music/" + filename).c_str());
	return mapa[name] != nullptr;
}
template <>
bool Almacen<Mix_Chunk*>::load(const std::string &name, const std::string &filename) {
	mapa[name] = Mix_LoadWAV(assetPath("audio/sfx/" + filename).c_str());
	return mapa[name] != nullptr;
}
template <>
void Almacen<SDL_Texture*>::clear() {
	for (auto iterator = mapa.begin(); iterator != mapa.end(); ++iterator) {
		SDL_DestroyTexture(iterator->second);
	}
	IMG_Quit();
}
template <>
void Almacen<Mix_Music*>::clear() {
	for (auto iterator = mapa.begin(); iterator != mapa.end(); ++iterator) {
		Mix_FreeMusic(iterator->second);
	}
	Mix_CloseAudio();
}
template <>
void Almacen<Mix_Chunk*>::clear() {
	for (auto iterator = mapa.begin(); iterator != mapa.end(); ++iterator) {
		Mix_FreeChunk(iterator->second);
	}
	while (Mix_Init(0)) Mix_Quit();
}