#include <set>

#include "ArenaRender.h"
#include "ArenaTest.h"

ArenaRender::ArenaRender(unsigned int ups, const Anwill::WindowSettings& ws)
    : Anwill::Layer(ups), m_Camera(ws.width, ws.height)
{
    m_Camera.Move(ws.width * 0.5f, ws.height * 0.5f);
}

void ArenaRender::Update(const Anwill::Timestamp& timestamp)
{
    float delta = GetUpdateDelta(timestamp).GetSeconds();

    Anwill::Renderer2D::BeginScene(m_Camera);

    Anwill::Ecs::ForEach<Anwill::Math::Mat4f, Anwill::RBody>([](Anwill::EntityID id,
            Anwill::Math::Mat4f& transform, Anwill::RBody& body) {
        Anwill::Math::Vec3f pos = body.GetPosition();
        transform.SetTranslation(pos);

        if(id == ArenaTest::s_Player) {
            Anwill::Math::Vec3f playerColor = {0.858f, 0.552f, 0.223f};
            if(ArenaTest::s_PlayerIsRound) {
                Anwill::Renderer2D::PushCircleToBatch(transform, playerColor);
            } else {
                Anwill::Renderer2D::PushQuadToBatch(transform, playerColor);
            }
        } else {
            if (transform.GetScale().X == transform.GetScale().Y) {
                Anwill::Math::Vec3f color = {0.258f, 0.852f, 0.223f};
                if(body.IsStatic()) {
                    color = {1.0f, 1.0f, 0.0f};
                }
                if (id % 2 == 0) {
                    Anwill::Renderer2D::PushCircleToBatch(transform, color);
                } else {
                    Anwill::Renderer2D::PushQuadToBatch(transform, color);
                }
            }
            Anwill::Renderer2D::DrawBatch();
        }
    });

    Layer::Update(timestamp);
}