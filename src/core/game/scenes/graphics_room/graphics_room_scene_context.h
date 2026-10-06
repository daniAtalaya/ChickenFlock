#pragma once

#include "game/game_scene.h"
#include "resources/game_assets.h"

#include <functional>

struct GraphicsRoomSceneContext {
	GameAssets& assets;
	SDL_Renderer* renderer;
	MIX_Mixer* mixer;
	SDL_Window* window;
	std::function<void(Escena)> changeScene;
};
