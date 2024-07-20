#pragma once

#include "engine/AssetManager.hpp"

#include <entt/fwd.hpp>

class TextSystem
{
public:
  TextSystem(entt::registry& registry, AssetManager& asset_manager);
  void Update();

private:
  entt::registry& m_registry;
  AssetManager&   m_asset_manager;
};
