#pragma once

#include <array>

#include "EcsDef.h"
#include "Entity.h"

namespace Anwill {

    using  = std::array<Entity, AW_MAX_ENTITIES>;

    class EntityManager
    {
    public:
        EntityManager();

        EntityID CreateEntity();
        bool IsAlive(EntityID entityID);
        void Kill(EntityID entityID);

    private:
        unsigned long m_AliveEntities;
        Entities m_Entities;
        EntityID m_EntityIDCounter;

        EntityID GenerateID();
    };
}