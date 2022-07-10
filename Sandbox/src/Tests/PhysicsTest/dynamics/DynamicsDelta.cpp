#include "DynamicsDelta.h"

DynamicsDelta::DynamicsDelta(unsigned int ups, const Anwill::WindowSettings& ws)
    : Anwill::Layer(ups), m_G(0.0f, -9.81f, 0.0f), m_Camera(ws.width, ws.height)
{
    m_ObjShader = Anwill::Shader::Create("assets/shaders/HelloUniform.glsl");
    Anwill::Ecs::RegisterComponent<Anwill::RBody>(); // TODO: Move to engine
}

void DynamicsDelta::Update(const Anwill::Timestamp& timestamp)
{
    float delta = timestamp.GetSeconds() - m_LastUpdate.GetSeconds();

    Anwill::Renderer::BeginScene(m_Camera);

    Anwill::Ecs::ForEach<Anwill::RBody, Anwill::TransformComponent, Anwill::MeshComponent>([delta, this](Anwill::EntityID id,
            Anwill::RBody& rBody,
            Anwill::TransformComponent& transformComponent,
            Anwill::MeshComponent& meshComponent) {

        transformComponent.transform = Anwill::Math::Mat4f::Translate(Anwill::Math::Mat4f::Identity(), rBody.GetPosition());
        Anwill::Renderer::Submit(m_ObjShader, meshComponent.mesh, transformComponent.transform);

        rBody.Tick(2 * delta);
    });

    Layer::Update(timestamp);
}
