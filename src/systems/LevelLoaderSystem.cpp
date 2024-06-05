#include "LevelLoaderSystem.hpp"

#include "../Events.hpp"

#include <algorithm> // std::transform
#include <cctype>    // std::tolower
#include <fstream>
#include <spdlog/spdlog.h>
#include <sstream>
#include <string_view>

LevelLoaderSystem::LevelLoaderSystem(entt::registry& registry, entt::dispatcher& dispatcher)
  : m_registry{registry}
  , m_dispatcher{dispatcher}
  , m_enemy_list_to_dispatch{}
{
}

void
LevelLoaderSystem::LoadLevel(std::string_view level_data_path)
{

  m_enemy_list_to_dispatch.clear();
  m_level_counter = 0;

  std::ifstream file_stream{level_data_path.data()};

  if (file_stream.is_open()) {
    std::string enemy_type_str, line_str;
    int         delay, pos_x, pos_y;
    while (std::getline(file_stream, line_str)) {

      // Skip empty lines and comments (beggining with #)
      if (std::all_of(line_str.cbegin(), line_str.cend(), [](char c) { return std::isspace(c); })) {
        spdlog::trace("Skipping empty line");
        continue;
      } else if (auto loc = line_str.find_first_not_of(" \n\t\r"); loc != std::string::npos and line_str[loc] == '#') {
        spdlog::trace("Skipping comment line: {}", line_str.c_str());
        continue;
      }

      // Parse the stream
      std::stringstream string_stream{line_str};
      if (string_stream >> enemy_type_str >> delay >> pos_x >> pos_y) {
        std::transform(enemy_type_str.begin(), enemy_type_str.end(), enemy_type_str.begin(), ::toupper);

        if (enemy_type_str == "SIMPLE") {
          m_enemy_list_to_dispatch.push_back({EnemyType::SIMPLE, delay, pos_x, pos_y});
        } else if (enemy_type_str == "SEEKER") {
          m_enemy_list_to_dispatch.push_back({EnemyType::SEEKER, delay, pos_x, pos_y});
        } else {
          spdlog::error("Enemy type {} not supported", enemy_type_str.data());
          m_enemy_list_to_dispatch.push_back({EnemyType::NOTYPE, delay, pos_x, pos_y});
        }
        spdlog::trace("Parsed line: {} {} {} {}", enemy_type_str.c_str(), delay, pos_x, pos_y);

      } else {
        spdlog::error("Could not parse line: {}", line_str.c_str());
      }
    }

    // Sort vector
    std::sort(
      m_enemy_list_to_dispatch.begin(),
      m_enemy_list_to_dispatch.end(), //
      [](const auto& a, const auto& b) { return a.delay > b.delay; });

  } else {
    // TODO: Throw error. Transform this into an assert
    spdlog::error("Error opening the level data file: {}", level_data_path.data());
  }
  file_stream.close();
}

void
LevelLoaderSystem::Update()
{

  if (not m_enemy_list_to_dispatch.empty()) {
    auto last = m_enemy_list_to_dispatch.back();
    while (last.delay <= m_level_counter) {
      EnemyUtils::CreateEnemy(m_registry, last.type, last.pos_x, last.pos_y);
      m_enemy_list_to_dispatch.pop_back();
      if (m_enemy_list_to_dispatch.empty())
        break;
      else
        last = m_enemy_list_to_dispatch.back();
    }
  } else // if all enemies have been dispatched, check if there are still enemies alive and if so send the end level
         // signal
  {
    if (not m_registry.view<Enemy>()) {
      m_dispatcher.enqueue(EndLevelEvent());
    }
  }
  m_level_counter++;
}

