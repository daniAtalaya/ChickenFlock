#include "almacen.h"
#include "game.h"

template <>
bool Almacen<SDL_Texture*>::load(std::string name, std::string filename) {
	mapa[name] = SDL_CreateTextureFromSurface(Game::renderer, IMG_Load(filename.c_str()));
	//std::cout << name << " --- " << filename << " --- " << SDL_GetError() << std::endl;
	return mapa[name] != NULL;
}
template <>
bool Almacen<Mix_Music*>::load(std::string name, std::string filename) {
	mapa[name] = Mix_LoadMUS(filename.c_str());
	//std::cout << name << " --- " << filename << " --- " << SDL_GetError() << std::endl;
	return mapa[name] != NULL;
}
template <>
bool Almacen<Mix_Chunk*>::load(std::string name, std::string filename) {
	mapa[name] = Mix_LoadWAV(filename.c_str());
	//std::cout << name << " --- " << filename << " --- " << SDL_GetError() << std::endl;
	return mapa[name] != NULL;
}
template <>
void Almacen<SDL_Texture*>::clear() {
	for (std::map<std::string, SDL_Texture*>::iterator iterador = mapa.begin(); iterador != mapa.end(); iterador++) {
		SDL_DestroyTexture(iterador->second);
	}
	IMG_Quit();
}
template <>
void Almacen<Mix_Music*>::clear() {
	for (std::map<std::string, Mix_Music*>::iterator iterador = mapa.begin(); iterador != mapa.end(); iterador++) {
		Mix_FreeMusic(iterador->second);
	}
	Mix_CloseAudio();
}
template <>
void Almacen<Mix_Chunk*>::clear() {
	for (std::map<std::string, Mix_Chunk*>::iterator iterador = mapa.begin(); iterador != mapa.end(); iterador++) {
		Mix_FreeChunk(iterador->second);
	}
	while (Mix_Init(0)) Mix_Quit();
}