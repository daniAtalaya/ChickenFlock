#pragma once

#include "lore_scene_context.h"
#include <memory>

std::unique_ptr<GameScene> createLoreScene(LoreSceneContext context);
