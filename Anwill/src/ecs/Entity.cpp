#include "Entity.h"

namespace Anwill {

    Entity::Entity(bool alive)
        : m_Alive(alive)
    {}

    /*
    Entity::Entity(unsigned long id)
        : m_ID(id), m_Alive(true) // TODO: I guess we start off alive? Or until user says so mby?
    {}
    */

    /*
    EntityID Entity::GetID()
    {
        return m_ID;
    }
     */

    bool Entity::IsAlive()
    {
        return m_Alive;
    }

    void Entity::Kill()
    {
        m_Alive = false;
    }
}