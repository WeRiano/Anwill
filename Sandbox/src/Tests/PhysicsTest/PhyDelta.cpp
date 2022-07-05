#include "PhyDelta.h"

PhyDelta::PhyDelta(unsigned int ups, const Anwill::WindowSettings& ws)
    : Anwill::Layer(ups), m_G(0.0f, -9.81f, 0.0f), m_Camera(ws.width, ws.height)
{
    m_ObjShader = Anwill::Shader::Create("assets/shaders/HelloUniform.glsl");
    Anwill::Ecs::RegisterComponent<Anwill::RBody>(); // TODO: Move to engine
}

void PhyDelta::Update(const Anwill::Timestamp& timestamp)
{
    float delta = timestamp.GetSeconds() - m_LastUpdate.GetSeconds();

    Anwill::Renderer::BeginScene(m_Camera);

    Anwill::Ecs::ForEach<Anwill::RBody, Anwill::MeshComponent>([delta, this](Anwill::EntityID id, Anwill::RBody& rBody,
            Anwill::MeshComponent& meshComponent) {
        Anwill::Math::Mat4f transform = Anwill::Math::Mat4f::Identity();
        transform.Translate(rBody.GetPosition());
        Anwill::Renderer::Submit(m_ObjShader, meshComponent.mesh, transform);

        //auto pos = rBody.GetPosition();
        //AW_INFO("Entity {0} pos: {1}, {2}", id, pos.GetX(), pos.GetY());

        rBody.Tick(2 * delta);
    });

    Layer::Update(timestamp);
}
