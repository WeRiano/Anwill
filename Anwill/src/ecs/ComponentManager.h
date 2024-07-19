#pragma once

#include <array>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <functional>
#include <vector>

#include "EcsDef.h"
#include "ComponentContainer.h"
#include "core/Log.h"
#include "core/Assert.h"

namespace Anwill {

    class ComponentManager
    {
    public:
        ComponentManager()
            : m_IDCounter(0)
        {}

        template <class C>
        void RegisterComponent()
        {
            std::type_index typeIndex = std::type_index(typeid(C));
            if(m_IDs.find(typeIndex) != m_IDs.end()) {
                AW_WARN("ECS: Component has already been registered.");
                return;
            }
            ComponentID id = CreateID();
            m_IDs[typeIndex] = id;
            m_Containers[id] = std::make_shared<ComponentContainer<C>>();
        }

        template <class C, typename... Args>
        void AddComponent(EntityID entityID, Args&&... args)
        {
            ComponentID id = GetTypeID<C>();
            if (m_Containers[id] == nullptr) {
                AW_ERROR("ECS: Tried to add a {0} component to the ECS but such "
                         "a component has not been registered.", typeid(C).name());
            } else
            {
                std::shared_ptr<ComponentContainer<C>> container =
                        GetComponentContainer<C>();
                container->AddComponent(entityID, std::forward<Args>(args)...);
                AW_INFO("ECS: {0} component added to entity {1}.",
                        typeid(C).name(), entityID);
            }
        }

        template <class C>
        void DeleteComponent(EntityID entityID)
        {
            ComponentID id = GetTypeID<C>();
            if (m_Containers[id] == nullptr) {
                AW_ERROR("ECS: Tried to remove a {0} component from the ECS but such a "
                         "component has not been registered.", typeid(C).name());
            } else {
                std::shared_ptr<ComponentContainer<C>> container =
                        GetComponentContainer<C>();
                container->DeleteComponent(entityID);
                AW_INFO("ECS: {0} component removed from entity {1}.", typeid(C).name(),
                        entityID);
            }
        }

        inline void DeleteComponent(EntityID entityID, ComponentID componentID) {
            if (m_Containers[componentID] == nullptr) {
                AW_ERROR("ECS: Tried to remove a component with id {0} from the ECS but "
                         "such a component has not been registered.",
                         componentID);
            } else {
                auto container = m_Containers[componentID];
                container->DeleteComponent(entityID);
                AW_INFO("ECS: A component with id {0} removed from entity {1}.",
                        componentID, entityID);
            }
        }

        template <size_t CompNr, class... Comps>
        void GetComponentsInOrder(const EntityID& entityID,
                                  const std::vector<ComponentID>& compIDs,
                                  std::tuple<Comps*...>& result)
        {
            // For efficiency, we should grab the components in TYPE order:
            // (Grab all components of type A for entity x1, x2, ..., xn)
            // (Grab all components of type B for entity x1, x2, ..., xn)
            // (...)
            using C = std::tuple_element_t<CompNr, std::tuple<Comps...>>;
            const ComponentID& id = compIDs[CompNr];
            std::shared_ptr<ComponentContainer<C>> container = GetComponentContainer<C>();

            C& c = container->GetComponent(entityID);
            std::get<CompNr>(result) = &c;

            if constexpr (CompNr < sizeof...(Comps) - 1)
            {
                GetComponentsInOrder<CompNr + 1, Comps...>(entityID, compIDs, result);
            }
        }

        template <size_t CompNr, class... Comps>
        void GetComponentsInOrder(const std::vector<EntityID>& entityIDs,
                                  const std::vector<ComponentID>& compIDs,
                                  std::vector<std::tuple<Comps*...>>& result)
        {
            // For efficiency, we should grab the components in TYPE order:
            // (Grab all components of type A for entity x1, x2, ..., xn)
            // (Grab all components of type B for entity x1, x2, ..., xn)
            // (...)
            using C = std::tuple_element_t<CompNr, std::tuple<Comps...>>;
            const ComponentID& id = compIDs[CompNr];
            std::shared_ptr<ComponentContainer<C>> container = GetComponentContainer<C>();
            unsigned int count = 0;
            for(const auto& entityID : entityIDs)
            {
                C& c = container->GetComponent(entityID);
                std::get<CompNr>(result[count]) = &c;
                count++;
            }
            if constexpr (CompNr < sizeof...(Comps) - 1)
            {
                GetComponentsInOrder<CompNr + 1, Comps...>(entityIDs, compIDs, result);
            }
        }

        template <class C>
        ComponentID GetTypeID()
        {
            return m_IDs[std::type_index(typeid(C))];
        }

        template <class C, class... Comps>
        void GetTypeIDs(std::vector<ComponentID>& result)
        {
            result.emplace_back(m_IDs[std::type_index(typeid(C))]);
            if constexpr (sizeof...(Comps) > 0)
            {
                GetTypeIDs<Comps...>(result);
            }
        }

    private:
        std::unordered_map<ComponentID, std::shared_ptr<IContainer>> m_Containers;
        std::unordered_map<std::type_index, ComponentID> m_IDs;

        unsigned int m_IDCounter;

        ComponentID CreateID()
        {
            m_IDCounter++;
            if(m_IDCounter > AW_MAX_TYPE_COMPONENTS) {
                AW_FATAL("ECS: Maximum of {0} unique components has been registered. "
                         "Cannot register another component.",
                         AW_MAX_TYPE_COMPONENTS);
            }
            return m_IDCounter;
        }

        template<class C>
        std::shared_ptr<ComponentContainer<C>> GetComponentContainer()
        {
            ComponentID& id = m_IDs[std::type_index(typeid(C))];
            AW_ASSERT(m_Containers[id] != nullptr,
                      "ECS: Attempted to access a component container of a type that has "
                      "not been registered.");
            return std::static_pointer_cast<ComponentContainer<C>>(
                    m_Containers[m_IDs[std::type_index(typeid(C))]]);
        }
    };

}