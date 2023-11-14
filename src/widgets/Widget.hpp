#pragma once

#include <functional>

class Widget
{
  public:
    virtual ~Widget() = default;
    virtual void SetActive(){};
    virtual void UnsetActive(){};
    virtual void TriggerAction() = 0;
};

//
// Connects the Action of the widget with a a callback from a member function of class T
//
class CallBackWidget : public Widget
{
  public:
    template<auto Candidate, typename T>
    void Connect(T* instance)
    {
        m_instance = instance;
        m_delegate = [](void* t_instance) -> void {
            T* casted = static_cast<T*>(t_instance);
            return std::invoke(Candidate, casted);
        };
    }

    void TriggerAction() { m_delegate(m_instance); }

  private:
    void* m_instance;
    void  (*m_delegate)(void*);
};
