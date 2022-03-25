#pragma once

#include <array>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <functional>
#include <vector>

#include "EcsDef.h"
#include "Component.h"

namespace Anwill {

    class ComponentManager
    {
    public:
        ComponentManager() {}

        template <class C, typename... Args>
        void AddComponent(EntityID entityID, Args&&... args)
        {
            std::shared_ptr<Component> ptr = std::make_shared<C>(std::forward<Args>(args)...);
            ComponentID id = GetTypeID<C>();
            m_CompIndexMap[entityID][id] = 0;
            m_Components[id][GetComponentArrayIndex(id, entityID)] = ptr;
            AW_INFO("ECS: {0} component added to entity {1}.", id.name(), entityID);
        }

        template<std::size_t N, class... Comps>
        bool PopulateTuple(std::tuple<Comps*...>& tuple,
                           EntityID entityID,
                           const std::vector<std::type_index>& indices) {
            auto* c = m_Components[indices[N]][entityID].get();
            if (c == nullptr) {
                return false;
            }
            std::get<N>(tuple) = (std::tuple_element_t<N, std::tuple<Comps*...>>)c;
            if constexpr (N < sizeof...(Comps) - 1) {
                return PopulateTuple<N + 1, Comps...>(tuple, entityID, indices);
            } else {
                return true;
            }
        }

        unsigned int GetComponentArrayIndex(ComponentID componentID, EntityID entityID) {
            return m_CompIndexMap[entityID][componentID];
        }

        template <class... Comps>
        bool GetComponents(EntityID entityID, std::tuple<Comps*...>& result)
        {
            auto compIndices = GetTypeIDs<Comps...>();
            return PopulateTuple<0, Comps...>(result, entityID, compIndices);
        }

    private:
        std::unordered_map<ComponentID, std::array<std::shared_ptr<Component>, AW_MAX_COMPONENTS>> m_Components;
        std::unordered_map<EntityID, std::unordered_map<ComponentID, unsigned int>> m_CompIndexMap;

        template <class C>
        ComponentID GetTypeID()
        {
            return std::type_index(typeid(C));
        }

        template <class... Comps>
        std::vector<ComponentID> GetTypeIDs()
        {
            return { std::type_index(typeid(Comps))... };
        }
    };

}