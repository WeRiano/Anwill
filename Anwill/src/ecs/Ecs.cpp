#include "Ecs.h"
#include "EcsDef.h"
#include "core/Log.h"

namespace Anwill {

    std::unique_ptr<Ecs> Ecs::s_Instance = nullptr;

    Ecs::Ecs()
        : m_EntityManager(), m_ComponentManager()
    {}

    void Ecs::Initialize()
    {
        if (s_Instance == nullptr)
        {
            s_Instance = std::make_unique<Ecs>();

            // TODO: Iterate? Lots of work for something so trivial ...
            s_Instance->RegisterComponent<MeshComponent>();
            s_Instance->RegisterComponent<CameraComponent>();
            s_Instance->RegisterComponent<TransformComponent>();
            AW_INFO("ECS Initialized!");
        } else {
            AW_WARN("ECS already initialized!");
        }
    }

    EntityID Ecs::CreateEntity()
    {
        return s_Instance->m_EntityManager.CreateEntity();
    }

}