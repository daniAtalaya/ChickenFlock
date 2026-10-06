#pragma once

#include "graphics_room_scene_context.h"
#include <memory>

std::unique_ptr<GameScene> createGraphicsRoomScene(GraphicsRoomSceneContext context);
