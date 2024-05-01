#include "LevelLoaderSystem.hpp"

#include "../Events.hpp"
#include "../Colors.hpp"

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
    std::transform(enemy_type_str.begin(),
                   enemy_type_str.end(),
                   enemy_type_str.begin(), //
                   [](unsigned char c) { return std::toupper(c); });

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
            if (std::all_of(line_str.cbegin(), line_str.cend(), [](char c) { return std::isspace(c); }))
            {
                spdlog::trace("Skipping empty line");
                continue;
            }
            else if (auto loc = line_str.find_first_not_of(" \n\t\r"); loc != std::string::npos and line_str[loc] == '#')
            {
                spdlog::trace("Skipping comment line: {}", line_str);
                continue;
            }

            std::stringstream string_stream{line_str};
            if (string_stream >> enemy_type_str >> delay >> pos_x >> pos_y)
            {
                m_enemy_list_to_dispatch.push_back(EnemyEntry{StrToEnemyType(enemy_type_str), delay, pos_x, pos_y});
                spdlog::trace("Parsed line: {} {} {} {}", enemy_type_str, delay, pos_x, pos_y);
            }
            else
            {
                spdlog::error("Could not parse line: {}", line_str);
            }
        }
        // Sort vector
        std::sort(m_enemy_list_to_dispatch.begin(),
                  m_enemy_list_to_dispatch.end(), //
                  [](const auto& a, const auto& b) { return a.delay > b.delay; });
    }
    else
    {
        spdlog::error("Error opening the level data file: {}", level_data_path);
        // TODO: Throw error
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
            CreateEnemy(m_registry, last.type, last.pos_x, last.pos_y);
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

void
LevelLoaderSystem::CreateEnemy(entt::registry& registry, EnemyType enemy_type, int pos_x, int pos_y)
{
    switch (enemy_type)
    {
        case EnemyType::SIMPLE: {
            auto entity = registry.create();
            // TODO: Put them initialy outside of the screen
            registry.emplace<Position>(entity, static_cast<float>(pos_x), static_cast<float>(pos_y));
            registry.emplace<Collider>(entity, 20, 20);
            registry.emplace<SquarePrimitive>(entity, 20, 20, Colors::BLUE);
            registry.emplace<Health>(entity, 5);
            registry.emplace<Weapon>(entity, 5);
            registry.emplace<Enemy>(entity, EnemyType::SIMPLE);
            spdlog::info("Creating new enemy entity {} of type SIMPLE", static_cast<int>(entity));
            break;
        }
        default:
            spdlog::error("Enemy Type = {} Case not implemented", static_cast<int>(enemy_type));
    }
}
