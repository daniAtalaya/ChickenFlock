#include "game_scene_manager.h"
#include "game_scene_creators.h"

void GameSceneManager::initialize(Escena scene, const GameSceneFactoryContext& context) {
	for (std::size_t index = 0; index < sceneCount; ++index) {
		scenes[index] = createGameScene(static_cast<Escena>(index), context);
	}
	currentScene = scene;
	publishedScene = &context.currentScene;
	gamesPlayed = &context.gamesPlayed;
	hardMode = &context.hardMode;
	playButton = &context.playButton;
	mouse = context.mouse;
	togglePause = context.togglePause;
	haltMusic = context.haltMusic;
	*publishedScene = scene;
	activeScene = scenes[static_cast<std::size_t>(scene)].get();
	if (activeScene != nullptr) {
		activeScene->enter(scene);
	}
}

void GameSceneManager::changeTo(Escena scene) {
	applyTransition(scene);
}

void GameSceneManager::applyTransition(Escena scene) {
	const Escena previousScene = currentScene;
	if (activeScene != nullptr) {
		activeScene->exit(scene);
	}

	if (previousScene == MENU && scene == LORE) {
		++*gamesPlayed;
	}
	if (previousScene == PAUSA && scene == MENU) {
		*hardMode = false;
	}
	if (previousScene != LORE && previousScene != PAUSA) {
		haltMusic();
	}

	currentScene = scene;
	*publishedScene = scene;
	activeScene = scenes[static_cast<std::size_t>(scene)].get();
	if (activeScene != nullptr) {
		activeScene->enter(previousScene);
	}
}

void GameSceneManager::handleInput(const SDL_Event& event) {
	if (activeScene != nullptr) {
		activeScene->handleInput(event);
	}
}

void GameSceneManager::handleClick() {
	if (activeScene == nullptr) {
		return;
	}
	activeScene->handleClick();
	if (playButton->isClicked(mouse)) {
		togglePause();
	}
}

void GameSceneManager::update() {
	if (activeScene != nullptr) {
		activeScene->update();
	}
}

void GameSceneManager::render() {
	if (activeScene != nullptr) {
		activeScene->render();
	}
}
