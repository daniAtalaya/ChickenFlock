#pragma once

#include "pause_scene_context.h"
#include <memory>

std::unique_ptr<GameScene> createPauseScene(PauseSceneContext context);
