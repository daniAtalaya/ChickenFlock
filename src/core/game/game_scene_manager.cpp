#include "game_scene_manager.h"
#include <SDL.h>
#include <stdexcept>
#include <utility>

void GameSceneManager::initialize(const Escena scene, SceneCollection sceneCollection,
	const GameSceneManagerContext& context) {
	if (static_cast<std::size_t>(scene) >= sceneCount) {
		throw std::invalid_argument("Cannot initialize the scene manager with an invalid scene");
	}
	for (std::size_t index = 0; index < sceneCount; ++index) {
		if (!sceneCollection[index] || sceneCollection[index]->id() != static_cast<Escena>(index)) {
			throw std::invalid_argument("Scene collection must contain exactly one correctly indexed scene");
		}
	}
	scenes = std::move(sceneCollection);
	state = scene;
	progress = &context.progress;
	haltMusic = context.haltMusic;
	saveProgress = context.saveProgress;
	activeScene = scenes[static_cast<std::size_t>(state)].get();
	if (activeScene != nullptr) {
		activeScene->enter(scene);
	}
}

void GameSceneManager::changeTo(const Escena scene) {
	if (static_cast<std::size_t>(scene) >= sceneCount) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Rejected transition to invalid scene %d", scene);
		return;
	}
	if (scene == state) {
		return;
	}
	if (!canTransitionTo(scene)) {
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Rejected invalid transition from %d to %d", state, scene);
		return;
	}
	applyTransition(scene);
}

bool GameSceneManager::canTransitionTo(const Escena scene) const {
	switch (state) {
	case INICI: return scene == MENU;
	case MENU: return scene == LORE || scene == TIENDA || scene == CREDITS || scene == GRAPHICS_ROOM;
	case LORE: return scene == JOC;
	case JOC: return scene == PAUSA || scene == GAMEOVER || scene == GUANYAT;
	case GAMEOVER: return scene == MENU;
	case GUANYAT: return scene == CREDITS;
	case TIENDA: return scene == MENU;
	case PAUSA: return scene == MENU || scene == JOC;
	case CREDITS: return scene == MENU;
	case GRAPHICS_ROOM: return scene == MENU;
	default: return false;
	}
}

void GameSceneManager::applyTransition(const Escena scene) {
	const Escena previousScene = state;
	if (activeScene != nullptr) {
		activeScene->exit(scene);
	}

	if (previousScene == MENU && scene == LORE) {
		++progress->gamesPlayed;
		saveProgress(*progress);
	}
	if (previousScene == PAUSA && scene == MENU) {
		hardMode = false;
		scenes[static_cast<std::size_t>(JOC)]->exit(MENU);
	}
	if (previousScene == GAMEOVER || previousScene == GUANYAT || previousScene == CREDITS) {
		hardMode = false;
	}
	const bool pausingGameplay = previousScene == JOC && scene == PAUSA;
	const bool resumingGameplay = previousScene == PAUSA && scene == JOC;
	if (previousScene != LORE && !pausingGameplay && !resumingGameplay) {
		haltMusic();
	}

	state = scene;
	activeScene = scenes[static_cast<std::size_t>(state)].get();
	if (activeScene != nullptr) {
		activeScene->enter(previousScene);
	}
}

void GameSceneManager::handleInput(const SDL_Event& event) const {
	if (activeScene != nullptr) {
		activeScene->handleInput(event);
	}
}

void GameSceneManager::handleClick(const SDL_Point& position) const {
	if (activeScene == nullptr) {
		return;
	}
	activeScene->handleClick(position);
}

void GameSceneManager::update() const {
	if (activeScene != nullptr) {
		activeScene->update();
	}
}

void GameSceneManager::render(SDL_Renderer* renderer, const bool showHitboxes) const {
	if (activeScene != nullptr) {
		activeScene->render(renderer, showHitboxes);
	}
}
