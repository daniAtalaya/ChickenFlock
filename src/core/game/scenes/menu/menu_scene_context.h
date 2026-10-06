#pragma once

#include "game/game_progress.h"
#include "game/game_scene.h"
#include "gameplay/camera.h"
#include "npc/perro/perro.h"
#include "player/player.h"
#include "resources/game_assets.h"
#include "ui/button.h"

#include <functional>
#include <string>

struct MenuSceneContext {
	GameAssets& assets;
	std::function<void()> haltChannels;
	std::function<void(const std::string&, int)> playMusic;
	GameProgress& progress;
	std::function<void()> initialize;
	std::function<void(Escena)> changeScene;
	std::function<void()> toggleMute;
	bool& muted;
	bool& debugHitboxes;
	bool& hardMode;
	Camera& camera;
	Player& player;
};
