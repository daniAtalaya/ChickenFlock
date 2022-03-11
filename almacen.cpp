#include "almacen.h"
#include "game.h"

bool Almacen<SDL_Texture*>::loadImage(std::string name, std::string filename)
{
	mapa[name] = SDL_CreateTextureFromSurface(Game::renderer, IMG_Load(filename.c_str()));
	return mapa[name] != NULL;
}
bool Almacen<Mix_Music*>::loadMusic(std::string name, std::string filename)
{
	mapa[name] = Mix_LoadMUS(filename.c_str());
	return mapa[name] != NULL;
}
bool Almacen<Mix_Chunk*>::loadEffect(std::string name, std::string filename)
{
	mapa[name] = Mix_LoadWAV(filename.c_str());
	return mapa[name] != NULL;
}