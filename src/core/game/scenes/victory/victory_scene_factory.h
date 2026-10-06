#pragma once

#include "victory_scene_context.h"
#include <memory>

std::unique_ptr<GameScene> createVictoryScene(VictorySceneContext context);
