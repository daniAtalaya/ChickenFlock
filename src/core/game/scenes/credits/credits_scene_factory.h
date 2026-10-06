#pragma once

#include "credits_scene_context.h"
#include <memory>

std::unique_ptr<GameScene> createCreditsScene(CreditsSceneContext context);
