#pragma once

#include "enums.h"
#include "game_world.h"
#include "resources/game_assets.h"
#include "ui/button.h"
#include "player/player.h"
#include "gameplay/camera.h"
#include "npc/pajaro/pajaro.h"
#include "npc/perro/perro.h"
#include "easter/avestruz/avestruz.h"

#include <functional>
#include <string>

struct GameSceneFactoryContext {
	SDL_Renderer* renderer;
	Escena& currentScene;
	GameAssets& assets;
	GameWorld& world;
	Player& player;
	Camera& camera;
	Pajaro& bird;
	Perro& pet;
	Avestruz& ostrich;
	Cuadrado& horda;
	Cuadrado& leftWall;
	Cuadrado& rightWall;
	Cuadrado& credits;
	Cuadrado& continueCard;
	Button& soundButton;
	Button& playButton;
	Button& shopButton;
	Button& backButton;
	Button& hardcoreButton;
	bool& godMode;
	bool& muted;
	bool& paused;
	bool& hardMode;
	bool& isClicking;
	SDL_Rect* mouse;
	const Uint8* keyboard;
	int& gamesPlayed;
	int& temporaryMoney;
	std::function<void(Escena)> changeScene;
	std::function<void()> initialize;
	std::function<void()> toggleMute;
	std::function<void()> togglePause;
	std::function<void(const std::string&, int)> playSound;
	std::function<void(const std::string&, int)> playMusic;
	std::function<void()> haltMusic;
	std::function<void()> haltChannels;
};
