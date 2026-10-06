#pragma once

#include "gameplay_scene_context.h"
#include <memory>

std::unique_ptr<GameScene> createGameplayScene(GameplaySceneContext context);
