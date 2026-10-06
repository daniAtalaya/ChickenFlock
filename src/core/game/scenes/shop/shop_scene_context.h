#pragma once

#include "game/game_progress.h"
#include "game/game_scene.h"
#include "gameplay/camera.h"
#include "player/player.h"
#include "resources/game_assets.h"
#include "ui/button.h"

#include <functional>
#include <string>

struct ShopSceneContext {
	GameAssets& assets;
	bool& muted;
	std::function<void(const std::string&, int)> playMusic;
	Player& player;
	GameProgress& progress;
	std::function<void(Escena)> changeScene;
	std::function<void()> toggleMute;
	Camera& camera;
};
