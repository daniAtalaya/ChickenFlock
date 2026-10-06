#pragma once

#include "menu_scene_context.h"
#include <memory>

std::unique_ptr<GameScene> createMenuScene(MenuSceneContext context);
