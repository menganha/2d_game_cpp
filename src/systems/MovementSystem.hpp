#ifndef INC_2D_CPP_ECS_GAME_MOVEMENTSYSTEM_HPP
#define INC_2D_CPP_ECS_GAME_MOVEMENTSYSTEM_HPP

#include <entt/entt.hpp>
#include "../Events.hpp"

class MovementSystem {

public:
    void Update(entt::registry & registry);
    void OnInputEvent(InputEvent input_event);
};


#endif //INC_2D_CPP_ECS_GAME_MOVEMENTSYSTEM_HPP
