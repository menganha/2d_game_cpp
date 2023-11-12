#pragma once

#include "Widget.hpp"

struct WidgetNode
{
  public:
    Widget&     widget;
    WidgetNode* next;
    WidgetNode* previous;
};

class WidgetContainer
{
  public:
    WidgetContainer();
    ~WidgetContainer();
    void AppendWidget(Widget& widget); // Always copy values if is an lvalue or move them if they are an rvalue
    void MoveToPrevious();
    void MoveToNext();
    void TriggerAction();
    void SetActive();

  private:
    WidgetNode* m_head;
    WidgetNode* m_tail;
    WidgetNode* m_current;
};
