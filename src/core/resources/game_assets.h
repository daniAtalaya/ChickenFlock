#pragma once

#include "almacen.h"

class GameAssets {
public:
	GameAssets() = default;
	GameAssets(const GameAssets&) = delete;
	GameAssets& operator=(const GameAssets&) = delete;
	GameAssets(GameAssets&&) = delete;
	GameAssets& operator=(GameAssets&&) = delete;

	Almacen<Mix_Chunk*> sfxs;
	Almacen<Mix_Music*> tracks;
	Almacen<SDL_Texture*> images;

	bool load(SDL_Renderer* renderer);
	void clear();

private:
	bool cleared = false;
};
