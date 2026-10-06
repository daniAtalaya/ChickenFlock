#pragma once

#include "game_over_scene_context.h"
#include <memory>

std::unique_ptr<GameScene> createGameOverScene(GameOverSceneContext context);
