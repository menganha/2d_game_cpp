#include "ButtonWidget.hpp"

#include "../Colors.hpp"
#include "../Components.hpp"

ButtonWidget::ButtonWidget(std::string_view label, int pos_x, int pos_y, entt::registry& registry)
    : m_registry{registry}
    , m_text_entity{m_registry.create()}
{
    m_registry.emplace<Position>(m_text_entity, static_cast<float>(pos_x), static_cast<float>(pos_y));
    m_registry.emplace<Text>(m_text_entity, std::string{label}, "fonts/PressStart2P.ttf", Colors::BLACK);
    UnsetActive();
}

ButtonWidget::~ButtonWidget()
{
    m_registry.destroy(m_text_entity);
}

void
ButtonWidget::SetActive()
{
    auto& text = m_registry.get<Text>(m_text_entity);
    text.color = Colors::BLACK;
}

void
ButtonWidget::UnsetActive()
{
    auto& text = m_registry.get<Text>(m_text_entity);
    text.color = Colors::DIMMED_BLACK;
}
