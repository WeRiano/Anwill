#pragma once

#include <memory>
#include <tuple>
#include <vector>

#include "ComponentManager.h"
#include "EntityManager.h"
#include "Entity.h"
#include "DefaultComponents.h"
#include "core/Log.h"

namespace Anwill {

    class Ecs
    {
    public:
        Ecs();

        static void Initialize();

        // Add an entity to the system and returns its unique ID;
        static EntityID CreateEntity();

        // Add a component to an entity. This will REPLACE a component if it already exists for a given entity.
        template <class C, typename... Args>
        static void AddComponent(EntityID entityID, Args&&... args)
        {
            s_Instance->m_ComponentManager.AddComponent<C>(entityID, std::forward<Args>(args)...);
        }

        template <class... Comps>
        static void ForEach(const std::function<void(EntityID, Comps&...)>& func)
        {
            for(EntityID entityID = 0; entityID < AW_MAX_ENTITIES; entityID++)
            {
                std::tuple<Comps*...> comps;
                if (s_Instance->m_ComponentManager.GetComponents<Comps...>(entityID, comps))
                {
                    std::apply([entityID, func](Comps*... comps) {
                        func(entityID, *comps...);
                    }, comps);
                } else {
                    continue;
                }
            }
        }

    private:
        EntityManager m_EntityManager;
        ComponentManager m_ComponentManager;

        static std::unique_ptr<Ecs> s_Instance;
    };
}