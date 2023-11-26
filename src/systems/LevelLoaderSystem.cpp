#include "LevelLoaderSystem.hpp"

#include "../Enemy.hpp"
#include "../Events.hpp"

#include <algorithm> // std::transform
#include <cctype>    // std::tolower
#include <fstream>
#include <iostream>
#include <spdlog/spdlog.h>
#include <sstream>
#include <string>

static EnemyType
StrToEnemyType(std::string& enemy_type_str)
{
    std::transform(
      enemy_type_str.begin(),
      enemy_type_str.end(),
      enemy_type_str.begin(), //
      [](unsigned char c) { return std::toupper(c); }
    );

    if (enemy_type_str.compare("simple"))
        return EnemyType::SIMPLE;
    else if (enemy_type_str == "parab")
        return EnemyType::PARAB;
    else
    {
        spdlog::error("Enemy type {} not supported", enemy_type_str);
        return EnemyType::NOTYPE;
    }
}

LevelLoaderSystem::LevelLoaderSystem(entt::registry& registry, entt::dispatcher& dispatcher)
  : m_registry{registry}
  , m_dispatcher{dispatcher}
  , m_enemy_list_to_dispatch{}
{}

void
LevelLoaderSystem::LoadLevel(std::string_view level_data_path)
{

    m_enemy_list_to_dispatch.clear();
    m_level_counter = 0;

    std::ifstream file_stream{level_data_path.data()};

    if (file_stream.is_open())
    {
        std::string enemy_type_str;
        int         delay, pos_x, pos_y;
        std::string line_str;
        while (std::getline(file_stream, line_str))
        {
            if (line_str.empty())
            {
                spdlog::debug("Skipping empty line");
                continue;
            }

            std::stringstream string_stream{line_str};
            if (string_stream >> enemy_type_str >> delay >> pos_x >> pos_y)
            {
                m_enemy_list_to_dispatch.push_back(EnemyEntry{StrToEnemyType(enemy_type_str), delay, pos_x, pos_y});
                spdlog::debug("Parsed line: {} {} {} {}", enemy_type_str, delay, pos_x, pos_y);
            }
            else
            {
                spdlog::error("Could not parse line: {}", line_str);
            }
        }
        // Sort vector
        std::sort(
          m_enemy_list_to_dispatch.begin(),
          m_enemy_list_to_dispatch.end(), //
          [](const auto& a, const auto& b) { return a.delay > b.delay; }
        );
    }
    else
    {
        spdlog::error("Error opening the level data file: {}", level_data_path);
    }
    file_stream.close();
}

void
LevelLoaderSystem::Update()
{

    if (not m_enemy_list_to_dispatch.empty())
    {
        auto last = m_enemy_list_to_dispatch.back();
        while (last.delay <= m_level_counter)
        {
            enemy::CreateEnemy(m_registry, last.type, last.pos_x, last.pos_y);
            m_enemy_list_to_dispatch.pop_back();
            if (m_enemy_list_to_dispatch.empty())
                break;
            else
                last = m_enemy_list_to_dispatch.back();
        }
        m_level_counter++;
    }
    else // if all enemies have been dispatched, check if there are still enemies alive and if so send the end level signal
    {
        if (not m_registry.view<Enemy>())
        {
            m_dispatcher.enqueue(EndLevelEvent());
        }
    }
}
