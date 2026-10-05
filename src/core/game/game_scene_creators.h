#pragma once

#include "game_scene.h"
#include "game_scene_factory_context.h"
#include <memory>

std::unique_ptr<GameScene> createIntroScene(const GameSceneFactoryContext&);
std::unique_ptr<GameScene> createMenuScene(const GameSceneFactoryContext&);
std::unique_ptr<GameScene> createLoreScene(const GameSceneFactoryContext&);
std::unique_ptr<GameScene> createGameplayScene(const GameSceneFactoryContext&);
std::unique_ptr<GameScene> createGameOverScene(const GameSceneFactoryContext&);
std::unique_ptr<GameScene> createVictoryScene(const GameSceneFactoryContext&);
std::unique_ptr<GameScene> createShopScene(const GameSceneFactoryContext&);
std::unique_ptr<GameScene> createPauseScene(const GameSceneFactoryContext&);
std::unique_ptr<GameScene> createCreditsScene(const GameSceneFactoryContext&);

std::unique_ptr<GameScene> createGameScene(Escena scene, const GameSceneFactoryContext& context);
