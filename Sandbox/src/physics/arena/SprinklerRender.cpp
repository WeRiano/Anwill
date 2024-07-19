#include "SprinklerRender.h"

SprinklerRender::SprinklerRender(unsigned int ups, const Anwill::WindowSettings& ws)
    : Anwill::Layer(ups),
      m_Camera(ws.width, ws.height)
{}

void SprinklerRender::Update(const Anwill::Timestamp& timestamp)
{
    auto delta = GetUpdateDelta(timestamp);
    Anwill::Renderer2D::BeginScene(m_Camera);

    Anwill::Ecs::ForEach<Anwill::RBody, Anwill::Math::Mat4f>([delta]
    (Anwill::EntityID id, Anwill::RBody& rBody, Anwill::Math::Mat4f& transform) {

        transform = Anwill::Math::Mat4f::Translate(transform,
                                                   rBody.GetPosition() - transform.GetTranslateVector());
        rBody.Tick(2.0f * delta.GetSeconds());
        Anwill::Renderer2D::PushQuadToBatch(transform, {0.5f, 0.3f, 0.1f});
    });

    Anwill::Renderer2D::DrawBatch();

    Layer::Update(timestamp);
}
