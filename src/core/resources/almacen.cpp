#include "almacen.h"
#include "asset_path.h"

template <>
bool Almacen<SDL_Texture*>::load(const std::string &name, const std::string &filename,
	SDL_Renderer* renderer, MIX_Mixer*, bool) {
	SDL_Surface* surface = IMG_Load(assetPath("images/" + filename).c_str());
	if (surface == nullptr) {
		return false;
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_DestroySurface(surface);
	if (texture == nullptr) {
		return false;
	}
	if (!SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST)) {
		SDL_DestroyTexture(texture);
		return false;
	}
	mapa[name] = texture;
	return true;
}

template <>
bool Almacen<MIX_Audio*>::load(const std::string &name, const std::string &filename,
	SDL_Renderer*, MIX_Mixer* mixer, bool streamed) {
	const std::string folder = streamed ? "audio/music/" : "audio/sfx/";
	mapa[name] = MIX_LoadAudio(mixer, assetPath(folder + filename).c_str(), !streamed);
	return mapa[name] != nullptr;
}

template <>
void Almacen<SDL_Texture*>::clear() {
	for (auto iterator = mapa.begin(); iterator != mapa.end(); ++iterator) {
		SDL_DestroyTexture(iterator->second);
	}
}

template <>
void Almacen<MIX_Audio*>::clear() {
	for (auto iterator = mapa.begin(); iterator != mapa.end(); ++iterator) {
		MIX_DestroyAudio(iterator->second);
	}
}