#pragma once

#include "game_scene.h"
#include "game_scene_factory_context.h"
#include <array>
#include <cstddef>
#include <functional>
#include <memory>

class GameSceneManager {
public:
	void initialize(Escena scene, const GameSceneFactoryContext& context);
	void changeTo(Escena scene);
	void handleInput(const SDL_Event& event);
	void handleClick();
	void update();
	void render();

private:
	void applyTransition(Escena scene);

	static constexpr std::size_t sceneCount = static_cast<std::size_t>(CREDITS) + 1;
	std::array<std::unique_ptr<GameScene>, sceneCount> scenes;
	GameScene* activeScene = nullptr;
	Escena currentScene = INICI;
	Escena* publishedScene = nullptr;
	int* gamesPlayed = nullptr;
	bool* hardMode = nullptr;
	Button* playButton = nullptr;
	SDL_Rect* mouse = nullptr;
	std::function<void()> togglePause;
	std::function<void()> haltMusic;
};
