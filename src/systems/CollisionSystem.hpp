#ifndef INC_2D_CPP_ECS_GAME_COLLISIONSYSTEM_HPP
#define INC_2D_CPP_ECS_GAME_COLLISIONSYSTEM_HPP

#include <entt/entt.hpp>
#include "../Events.hpp"
#include "../Components.hpp"

class CollisionSystem {

public:
    CollisionSystem(entt::registry& registry, entt::dispatcher& dispatcher);

    void Update();

    void OnCollisionEvent(CollisionEvent);


private:
    entt::registry& m_registry;
    entt::dispatcher & m_dispatcher;
    static bool HasCollided(Position pos_a, Collider coll_a, Position pos_b, Collider coll_b);
};


#endif //INC_2D_CPP_ECS_GAME_COLLISIONSYSTEM_HPP
