#pragma once

#include <set>

#include "EcsDef.h"
#include "core/Assert.h"

namespace Anwill {

    // This only exist so that we don't have to define <C> when we define the outer map in ComponentManager.
    class IContainer {
    public:
        // We need this virtual base function since we need a delete function that is not
        // based on T type
        virtual void DeleteComponent(const EntityID& entityID) = 0;
        // Get number of components in this container.
        virtual unsigned int GetSize() = 0;
    };

    template <class C>
    class ComponentContainer : public IContainer {
    public:
        ComponentContainer()
            : m_NrElements(0)
        {}

        template <typename... Args>
        void AddComponent(const EntityID& entityID, Args&&... args)
        {
            m_Data[m_NrElements] = C(args...);
            m_EntityToIndex[entityID] = m_NrElements;
            m_IndexToEntity[m_NrElements] = entityID;
            m_NrElements++;
        }

        // Delete a component which belongs to an entity.
        // This operation is currently the "expensive" one because we have to plug the
        // hole. I feel like deleting should not be done too often though so this is
        // probably the best solution.
        inline void DeleteComponent(const EntityID& entityID) override
        {
            AW_ASSERT(m_EntityToIndex.contains(entityID),
                      "Tried to delete a component from an entity that doesn't have it.");
            unsigned int delIndex = m_EntityToIndex[entityID];
            m_EntityToIndex.erase(entityID);

            // The plugger is the right most component
            m_Data[delIndex] = m_Data[m_NrElements-1];

            // Get the entity connected to the plugger
            EntityID pluggerEntity = m_IndexToEntity[m_NrElements-1];

            // Make sure that plugger data still belongs to the right entity
            m_EntityToIndex[pluggerEntity] = delIndex;

            // Make sure that the hole index points to the plugger entity
            m_IndexToEntity[delIndex] = pluggerEntity;

            // Remove old index-to-entity-plugger map
            m_IndexToEntity.erase(m_NrElements-1);

            m_NrElements--;
        }

        inline C& GetComponent(const EntityID& entityID) {
            AW_ASSERT(m_EntityToIndex.contains(entityID),
                      "Tried to get a component owned by entity {0}, but that entity"
                      "does not own a component of that type.");
            return m_Data[m_EntityToIndex[entityID]];
        }

        inline unsigned int GetSize() override
        {
            return m_NrElements;
        }
    private:
        std::array<C, AW_MAX_COMPONENTS> m_Data;
        unsigned int m_NrElements;
        // Entity to index mapping so that the data is always packed in m_Data.
        std::unordered_map<EntityID, unsigned int> m_EntityToIndex;
        // We need IndexToEntity mapping for deletion,
        // since we need to know what entity the plugger component belongs to.
        std::unordered_map<unsigned int, EntityID> m_IndexToEntity;
    };
}