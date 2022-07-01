#include <algorithm>

#include "EntityManager.h"
#include "core/Assert.h"

namespace Anwill {

    EntityManager::EntityManager()
    {
        for(EntityID entityID = 0; entityID < AW_MAX_ENTITIES; entityID++)
        {
            m_AvailableIDs.emplace(entityID);
        }
    }

    EntityID EntityManager::CreateEntity()
    {
        AW_ASSERT(m_AliveEntities.size() < AW_MAX_ENTITIES, "ECS is at maximum entity capacity!");

        EntityID id = GenerateID();
        m_AliveEntities.insert(id); // Alive on creation
        return id;
    }

    bool EntityManager::IsAlive(EntityID entityID)
    {
        AW_ASSERT(entityID < AW_MAX_ENTITIES, "Illegitimate entity ID.");
        return m_AliveEntities.contains(entityID);
    }

    void EntityManager::Kill(EntityID entityID)
    {
        AW_ASSERT(entityID < AW_MAX_ENTITIES, "Illegitimate entity ID.");
        AW_ASSERT(m_AliveEntities.contains(entityID), "\"What is dead may never die.\"");
        m_AliveEntities.erase(entityID);
        m_CompSigs[entityID].reset();
        m_AvailableIDs.emplace(entityID);
    }

    void EntityManager::AddComponent(const EntityID& entityID, const ComponentID& componentID)
    {
        AW_ASSERT(m_AliveEntities.contains(entityID), "Entity does not exist.");
        AW_ASSERT((1 <= componentID) && (componentID <= AW_MAX_TYPE_COMPONENTS), "Illegitimate component id.");
        m_CompSigs[entityID].set(componentID);
    }

    void EntityManager::RemoveComponent(const EntityID &entityID, const ComponentID &componentID)
    {
        AW_ASSERT(m_AliveEntities.contains(entityID), "Entity does not exist.");
        AW_ASSERT((1 <= componentID) && (componentID <= AW_MAX_TYPE_COMPONENTS), "Illegitimate component id.");
        m_CompSigs[entityID].set(componentID, false);
    }

    std::vector<EntityID> EntityManager::GetEntitiesWithComponents(const std::vector<ComponentID>& compIDs)
    {
        std::vector<EntityID> result;
        std::for_each(m_AliveEntities.begin(),
                      m_AliveEntities.cend(),
                      [this, &result, compIDs](EntityID id) {
            if (HasComponents(id, compIDs)) {
                result.push_back(id);
            }
        });
        return result;
    }

    bool EntityManager::HasComponents(const EntityID& entityID,
                                      const std::vector<ComponentID>& compIDs) {
        AW_ASSERT(m_AliveEntities.contains(entityID), "Entity does not exist.");
        for(const auto& id : compIDs)
        {
            AW_ASSERT((1 <= id) && (id <= AW_MAX_TYPE_COMPONENTS), "Illegitimate component id.");
            if (!m_CompSigs[entityID].test(id)) {
                return false;
            }
        }
        return true;
    }

    EntityID EntityManager::GenerateID()
    {
        AW_ASSERT(!m_AvailableIDs.empty(), "There is no available Entity ID for generation.");

        EntityID id = m_AvailableIDs.front();
        m_AvailableIDs.pop();
        return id;
    }

}