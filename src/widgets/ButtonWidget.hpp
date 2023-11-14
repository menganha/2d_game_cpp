#pragma once

#include "Widget.hpp"

#include <entt/entt.hpp>
#include <string_view>

class ButtonWidget : public CallBackWidget
{
  public:
    ButtonWidget(std::string_view label, int pos_x, int pos_y, entt::registry& registry);
    ~ButtonWidget();
    void SetActive() override;
    void UnsetActive() override;

  private:
    entt::registry& m_registry;
    entt::entity    m_text_entity;
};
