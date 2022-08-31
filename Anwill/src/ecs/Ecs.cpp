#include "Ecs.h"
#include "EcsDef.h"
#include "core/Log.h"

namespace Anwill {

    std::unique_ptr<Ecs> Ecs::s_Instance = nullptr;

    Ecs::Ecs()
        : m_EntityManager(), m_ComponentManager()
    {}

    void Ecs::Init()
    {
        if (s_Instance == nullptr)
        {
            s_Instance = std::make_unique<Ecs>();

            //s_Instance->RegisterComponent<MeshComponent>();
            //s_Instance->RegisterComponent<CameraComponent>();
            //s_Instance->RegisterComponent<TransformComponent>();
            // TODO: "Static" system components??

            AW_INFO("ECS Initialized!");
        } else {
            AW_WARN("ECS already initialized!");
        }
    }

    EntityID Ecs::CreateEntity()
    {
        return s_Instance->m_EntityManager.CreateEntity();
    }

    void Ecs::RemoveEntity(EntityID entityID)
    {
        if( !s_Instance->m_EntityManager.IsAlive(entityID) )
        {
            return;
        }
        // Get all components connected to the entity
        CompSig sig = s_Instance->m_EntityManager.GetEntityComponents(entityID);
        // Remove the entity
        s_Instance->m_EntityManager.Kill(entityID);

        // Remove all the components
        const unsigned int nrComponents = sig.count();
        unsigned int count = 0;
        for(unsigned int compID = 0; compID < AW_MAX_TYPE_COMPONENTS; compID++)
        {
            if(sig[compID]) {
                s_Instance->m_ComponentManager.DeleteComponent(entityID, compID);
                count++;
            }
            if (count == nrComponents) {
                break;
            }
        }
    }

    bool Ecs::IsEntityAlive(EntityID entityID)
    {
        return s_Instance->m_EntityManager.IsAlive(entityID);
    }
}