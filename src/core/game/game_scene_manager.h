#pragma once

#include "game_scene.h"
#include "game_progress.h"
#include "ui/button.h"
#include <array>
#include <cstddef>
#include <functional>
#include <memory>

struct GameSceneManagerContext {
	GameProgress& progress;
	std::function<void()> haltMusic;
	std::function<void(const GameProgress&)> saveProgress;
};

class GameSceneManager {
public:
	static constexpr std::size_t sceneCount = static_cast<std::size_t>(GRAPHICS_ROOM) + 1;
	using SceneCollection = std::array<std::unique_ptr<GameScene>, sceneCount>;

	void initialize(Escena scene, SceneCollection scenes, const GameSceneManagerContext& context);
	void changeTo(Escena scene);
	void handleInput(const SDL_Event& event) const;
	void handleClick(const SDL_Point& position) const;
	void update() const;
	void render(SDL_Renderer* renderer, bool showHitboxes) const;
	Escena currentScene() const { return state; }
	bool& hardModeState() { return hardMode; }

private:
	bool canTransitionTo(Escena scene) const;
	void applyTransition(Escena scene);

	SceneCollection scenes;
	GameScene* activeScene = nullptr;
	Escena state = INICI;
	GameProgress* progress = nullptr;
	bool hardMode = false;
	std::function<void()> haltMusic;
	std::function<void(const GameProgress&)> saveProgress;
};
