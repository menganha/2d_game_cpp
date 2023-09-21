#ifndef INC_2D_CPP_ECS_GAME_MOVEMENTSYSTEM_HPP
#define INC_2D_CPP_ECS_GAME_MOVEMENTSYSTEM_HPP

#include <entt/entt.hpp>
#include "../Events.hpp"

class MovementSystem {

public:
    MovementSystem(entt::registry& registry);

    void Update();

    void OnDirectionalButtonEvent(DirectionalButtonEvent input_event);

private:
    entt::registry& m_registry;
};


#endif //INC_2D_CPP_ECS_GAME_MOVEMENTSYSTEM_HPP
