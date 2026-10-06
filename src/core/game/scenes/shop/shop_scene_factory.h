#pragma once

#include "shop_scene_context.h"
#include <memory>

std::unique_ptr<GameScene> createShopScene(ShopSceneContext context);
