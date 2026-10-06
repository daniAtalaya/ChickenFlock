#pragma once

#include "game/game_scene.h"
#include "resources/game_assets.h"
#include "ui/button.h"

#include <functional>

struct IntroSceneContext {
	GameAssets& assets;
	std::function<void(Escena)> changeScene;
	std::function<void()> toggleMute;
	bool& muted;
};
