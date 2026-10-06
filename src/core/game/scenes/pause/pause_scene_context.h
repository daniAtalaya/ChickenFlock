#pragma once

#include "game/game_scene.h"
#include "gameplay/camera.h"
#include "player/player.h"
#include "resources/game_assets.h"
#include "ui/button.h"

#include <functional>

struct PauseSceneContext {
	GameAssets& assets;
	bool& muted;
	std::function<void()> haltChannels;
	std::function<void(Escena)> changeScene;
	std::function<void()> toggleMute;
	Camera& camera;
	Player& player;
	std::function<void()> togglePause;
};
