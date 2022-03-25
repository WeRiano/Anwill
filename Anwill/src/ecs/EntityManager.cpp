#include "EntityManager.h"
#include "core/Assert.h"

namespace Anwill {

    EntityManager::EntityManager()
        : m_AliveEntities(0), m_EntityIDCounter(0)
    {

    }

    EntityID EntityManager::CreateEntity()
    {
        EntityID id = GenerateID();
        m_Entities[id] = Entity(true);
        m_AliveEntities++;
        return id;
    }

    bool EntityManager::IsAlive(EntityID entityID)
    {
        return m_Entities[entityID].IsAlive();
    }

    void EntityManager::Kill(EntityID entityID)
    {
        m_Entities[entityID].Kill();
        m_AliveEntities--;
    }

    EntityID EntityManager::GenerateID()
    {
        if (m_AliveEntities == AW_MAX_ENTITIES) {
            AW_ERROR("ECS is at maximum entity capacity!");
            return -1;
        }
        while (m_Entities[m_EntityIDCounter].IsAlive()) {
            m_EntityIDCounter = (m_EntityIDCounter + 1) % AW_MAX_ENTITIES;
        }
        EntityID res = m_EntityIDCounter;
        m_EntityIDCounter++;
        return res;
    }

}