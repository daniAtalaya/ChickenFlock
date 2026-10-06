#pragma once

#include "almacen.h"

class GameAssets {
public:
	GameAssets() = default;
	GameAssets(const GameAssets&) = delete;
	GameAssets& operator=(const GameAssets&) = delete;
	GameAssets(GameAssets&&) = delete;
	GameAssets& operator=(GameAssets&&) = delete;

	Almacen<MIX_Audio*> sfxs;
	Almacen<MIX_Audio*> tracks;
	Almacen<SDL_Texture*> images;

	bool load(SDL_Renderer* renderer, MIX_Mixer* mixer);
	void clear();

private:
	bool cleared = false;
};
