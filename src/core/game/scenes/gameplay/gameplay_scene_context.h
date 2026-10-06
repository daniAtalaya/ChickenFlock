#pragma once

#include "game/game_progress.h"
#include "game/game_scene.h"
#include "gameplay/camera.h"
#include "player/player.h"
#include "resources/game_assets.h"

#include <functional>
#include <string>

struct GameplaySceneContext {
	GameAssets& assets;
	bool& muted;
	std::function<void(const std::string&, int)> playMusic;
	std::function<void(const std::string&, int)> playSound;
	bool& hardMode;
	Player& player;
	Camera& camera;
	const bool* keyboard;
	GameProgress& progress;
	std::function<void(Escena)> changeScene;
	std::function<void()> toggleMute;
	std::function<void()> togglePause;
	bool& debugHitboxes;
};
