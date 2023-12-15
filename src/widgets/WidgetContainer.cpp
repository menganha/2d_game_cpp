#include "WidgetContainer.hpp"

WidgetContainer::WidgetContainer() : m_head{nullptr}, m_tail{nullptr}, m_current{nullptr} {}

WidgetContainer::~WidgetContainer()
{
    m_current = m_head;
    while (m_current != nullptr){
        m_current = m_current->previous;
        delete m_head;
        m_head = m_current;
    }
    m_tail = nullptr;
}

void
WidgetContainer::AppendWidget(Widget& widget)
{
    auto node = new WidgetNode{widget, nullptr, nullptr};
    if (m_head == nullptr)
    {
        m_head = node;
        m_tail = node;
        m_current = node;
        node->widget.SetActive();
    }
    else
    {
        node->previous = m_head;
        m_head->next = node;
        m_head = node;
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
