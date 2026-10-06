#pragma once

#include "intro_scene_context.h"
#include <memory>

std::unique_ptr<GameScene> createIntroScene(IntroSceneContext context);
