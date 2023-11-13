#include "WidgetContainer.hpp"

WidgetContainer::WidgetContainer() : m_head{ nullptr }, m_tail{ nullptr }, m_current{ nullptr } {}

WidgetContainer::~WidgetContainer()
{
    auto* tmp_node = m_tail;
    while (tmp_node != nullptr)
    {
        delete tmp_node;
        tmp_node = tmp_node->next;
    }
    m_head = nullptr;
    m_tail = nullptr;
}

void
WidgetContainer::AppendWidget(Widget& widget)
{
    auto node = new WidgetNode{ widget, nullptr, nullptr };
    node->next = nullptr;
    node->previous = nullptr;
    if (m_head == nullptr)
    {
        m_head = node;
        m_tail = node;
        m_current = node;
        node->widget.SetActive();
    }
    else
    {
        node->previous = m_tail;
        m_tail->next = node;
        m_tail = node;
    }
}

void
WidgetContainer::MoveToPrevious()
{
    if (m_current && m_current->previous)
    {
        m_current->widget.UnsetActive();
        m_current = m_current->previous;
        m_current->widget.SetActive();
    }
}

void
WidgetContainer::MoveToNext()
{
    if (m_current && m_current->next)
    {
        m_current->widget.UnsetActive();
        m_current = m_current->next;
        m_current->widget.SetActive();
    }
}

void
WidgetContainer::TriggerAction()
{
    if (m_current)
        m_current->widget.TriggerAction();
}
