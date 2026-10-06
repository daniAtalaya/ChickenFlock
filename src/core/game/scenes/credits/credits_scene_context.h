#pragma once

#include "game/game_scene.h"
#include "easter/avestruz/avestruz.h"
#include "resources/game_assets.h"
#include "ui/button.h"

#include <functional>
#include <string>

struct CreditsSceneContext {
	GameAssets& assets;
	bool& muted;
	std::function<void()> haltChannels;
	std::function<void(const std::string&, int)> playMusic;
	std::function<void(Escena)> changeScene;
	std::function<void()> toggleMute;
};
