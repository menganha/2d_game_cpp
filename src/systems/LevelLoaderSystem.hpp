#pragma once

#include "../AssetManager.hpp"
#include "../Components.hpp"

#include <entt/entt.hpp>
#include <string_view>

class LevelLoaderSystem
{
public:
    LevelLoaderSystem(entt::registry& registry, entt::dispatcher& dispatcher);
    void LoadLevel(std::string_view level_data_path);
    void Update();

private:
    //
    //  Enemy entry for when we want to specify the enemies on a given level
    //
    struct EnemyEntry
    {
        EnemyType type;
        int       delay;
        int       pos_x;
        int       pos_y;
    };
    using EnemyList = std::vector<EnemyEntry>;
    entt::registry&   m_registry;
    entt::dispatcher& m_dispatcher;
    EnemyList         m_enemy_list_to_dispatch;
    int m_level_counter; // FIXME: Could this cause an overflow at some point? Levels should not be so large I guess (?)
};
