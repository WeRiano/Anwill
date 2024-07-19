#pragma once

#include <memory>
#include <tuple>
#include <vector>

#include "ComponentManager.h"
#include "EntityManager.h"
#include "DefaultComponents.h"
#include "core/Log.h"

namespace Anwill {

    struct EcsStats {
        unsigned int numAliveEntities = 0;
        unsigned int numRegisteredComponents = 0;
        unsigned int numComponents = 0;
    };

    class Ecs
    {
    public:
        Ecs();

        static void Init();

        static void Reset();

        /**
         * Add an entity to the system and returns its unique ID;
         */
        static EntityID CreateEntity();

        /**
         * Remove an entity from the system and all its components, freeing its ID.
         */
        static void RemoveEntity(EntityID entityID);

        /**
         * Check if an entity is still alive
         */
        static bool IsEntityAlive(EntityID entityID);

        /**
         * Get ECS stats. See @EcsStats for more.
         */
        static EcsStats GetStats();

        /**
         * Register a component (type) to the ECS. A component which has not been
         * registered can not be used.
         */
        template <class C>
        static void RegisterComponent()
        {
            s_Instance->m_ComponentManager.RegisterComponent<C>();
        }

        /**
         * Add a component to an entity. This will replace any pre-existing component of the same type.
         */
        template <class C, typename... Args>
        static void AddComponent(EntityID entityID, Args&&... args)
        {
            s_Instance->m_ComponentManager.AddComponent<C>(entityID,
                                                           std::forward<Args>(args)...);
            s_Instance->m_EntityManager.AddComponent(entityID,
                                         s_Instance->m_ComponentManager.GetTypeID<C>());
        }

        template <class C>
        static void DeleteComponent(EntityID entityID)
        {
            s_Instance->m_ComponentManager.DeleteComponent<C>(entityID);
            s_Instance->m_EntityManager.RemoveComponent(entityID,
                                        s_Instance->m_ComponentManager.GetTypeID<C>());
        }

        template <class... Comps>
        static void ForEntity(EntityID entityID, const std::function<void(Comps&...)>& func)
        {
            std::vector<ComponentID> compIDs;
            // First we get the ids of the given components (Comps...)
            s_Instance->m_ComponentManager.GetTypeIDs<Comps...>(compIDs);

            if(s_Instance->m_EntityManager.HasComponents(entityID, compIDs))
            {

                std::tuple<Comps* ...> comps;
                s_Instance->m_ComponentManager.GetComponentsInOrder<0, Comps...>(entityID,
                                                                         compIDs, comps);

                std::apply([func, entityID](Comps* ... comps) {
                    func(*comps...);
                }, comps);
            }
        }

        template <class... Comps>
        static void ForEach(const std::function<void(EntityID, Comps&...)>& func)
        {
            std::vector<ComponentID> compIDs;
            // First we get the ids of the given components (Comps...)
            s_Instance->m_ComponentManager.GetTypeIDs<Comps...>(compIDs);

            // Then we get all the entities that have such components
            auto entities = s_Instance->m_EntityManager.GetEntitiesWithComponents(compIDs);

            // Since we are getting the components in a very weird fashion we
            // populate the tuple vector pre-emptively.
            // This however requires default constructor for all Entities.
            std::vector<std::tuple<Comps*...>> comps;
            for(unsigned int i = 0; i < entities.size(); i++)
            {
                comps.push_back(std::tuple<Comps*...>());
            }
            // We then grab all of these components
            s_Instance->m_ComponentManager.GetComponentsInOrder<0, Comps...>(entities,
                                                                             compIDs,
                                                                             comps);

            // Final step is to call the system function
            // (Each tuple is aligned with entityID)
            for(unsigned int i = 0; i < entities.size(); i++)
            {
                EntityID entity = entities[i];
                // We might remove an entity during callback func so lets be sure it still
                // exists
                if(!s_Instance->m_EntityManager.IsAlive(entity))
                {
                    continue;
                }
                std::apply([func, entity](Comps*... comps){
                    func(entity, *comps...);
                }, comps[i]);
            }
        }

    private:
        EntityManager m_EntityManager;
        ComponentManager m_ComponentManager;

        static std::unique_ptr<Ecs> s_Instance;
    };
}