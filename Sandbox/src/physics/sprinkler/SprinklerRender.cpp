#include "SprinklerRender.h"

SprinklerRender::SprinklerRender(unsigned int ups, const Anwill::WindowSettings& ws)
    : Anwill::Layer(ups),
      m_Camera(ws.width, ws.height)
{
    m_Camera.Move(ws.width * 0.5f, ws.height * 0.5f);

    Anwill::Renderer::SetClearColor({0.1f, 0.1f, 0.1f});
}

void SprinklerRender::Update(const Anwill::Timestamp& timestamp)
{
    auto delta = GetUpdateDelta(timestamp);
    Anwill::Renderer2D::BeginScene(m_Camera);

    Anwill::Ecs::ForEach<Anwill::RBody, Anwill::Math::Mat4f>([]
    (Anwill::EntityID id, Anwill::RBody& rBody, Anwill::Math::Mat4f& transform) {
        transform.SetTranslation(rBody.GetPosition());
        Anwill::Renderer2D::PushQuadToBatch(transform, {0.5f, 0.3f, 0.1f});
    });

    Anwill::Renderer2D::DrawBatch();

    Layer::Update(timestamp);
}
