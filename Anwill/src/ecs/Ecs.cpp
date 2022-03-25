#include "Ecs.h"
#include "EcsDef.h"
#include "core/Log.h"

namespace Anwill {

    std::unique_ptr<Ecs> Ecs::s_Instance = nullptr;

    Ecs::Ecs()
    {

    }

    void Ecs::Initialize()
    {
        if (s_Instance == nullptr)
        {
            s_Instance = std::make_unique<Ecs>();
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