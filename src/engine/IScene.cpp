#include "IScene.hpp"

SceneManager::SceneManager() : m_top_index{-1}, m_scene_stack{nullptr} {};

int
SceneManager::PushScene(std::shared_ptr<IScene> scene)
{
    if (m_top_index + 1 == kMaxSceneNum)
    {
        return 0; // Reached max number of scenes
    }
    else
    {
        m_top_index += 1;
        m_scene_stack[m_top_index] = scene;
        return 1;
    }
}

int
SceneManager::PopScene()
{
    if (IsEmpty())
    {
        return 0; // Emptying an empty container
    }
    else
    {
        m_scene_stack[m_top_index] = nullptr;
        m_top_index--;
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
