#pragma once

#include "game/game_scene.h"
#include "resources/game_assets.h"

#include <functional>

struct GraphicsRoomSceneContext {
	GameAssets& assets;
	SDL_Renderer* renderer;
	std::function<void(Escena)> changeScene;
};
