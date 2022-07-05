#pragma once

#include <bitset>
#include <array>
#include <set>
#include <queue>

#include "EcsDef.h"

namespace Anwill {

    class EntityManager
    {
    public:
        EntityManager();

        // Create a new alive unique entity.
        EntityID CreateEntity();

        // Check if a unique entity is alive.
        bool IsAlive(EntityID entityID);

        // Kill a unique entity.
        void Kill(EntityID entityID);

        // Link a specific component type to a unique entity.
        void AddComponent(const EntityID& entityID, const ComponentID& componentID);

        // Remove a specific component relationship to a unique entity.
        void RemoveComponent(const EntityID& entityID, const ComponentID& componentID);

        // Return all entities that have all the given components.
        std::vector<EntityID> GetEntitiesWithComponents(const std::vector<ComponentID>& comps);

        // Return all components of a given entity.
        CompSig GetEntityComponents(EntityID entityID);

    private:
        std::set<EntityID> m_AliveEntities;
        std::array<CompSig, AW_MAX_ENTITIES> m_CompSigs;
        std::queue<EntityID> m_AvailableIDs;

        // Check if a specific component type is linked to a unique entity.
        bool HasComponents(const EntityID& entityID, const std::vector<ComponentID>& comps);

        // Generate a new unique valid entity id.
        EntityID GenerateID();
    };
}