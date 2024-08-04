#include "IScene.hpp"

SceneManager::SceneManager()
  : m_top_index{-1}
  , m_scene_stack{nullptr} {};

int
SceneManager::PushScene(std::shared_ptr<IScene> scene)
{
  if (m_top_index + 1 == kMaxSceneNum) {
    return 0; // Reached max number of scenes
  } else {
    if (m_top_index >= 0)
      m_scene_stack[m_top_index]->OnExit();
    m_top_index += 1;
    m_scene_stack[m_top_index] = scene;
    scene->OnEnter();
    return 1;
  }
}

int
SceneManager::PopScene()
{
  if (IsEmpty()) {
    return 0; // Emptying an empty container
  } else {
    m_scene_stack[m_top_index]->OnExit();
    m_scene_stack[m_top_index] = nullptr;
    m_top_index--;
    if (m_top_index >= 0)
      m_scene_stack[m_top_index]->OnEnter();
    return 1;
  }
}

int
SceneManager::IsEmpty()
{
  if (m_top_index == -1)
    return 1;
  else
    return 0;
}

std::shared_ptr<IScene>
SceneManager::CurrentScene()
{
  return m_scene_stack[m_top_index];
}
