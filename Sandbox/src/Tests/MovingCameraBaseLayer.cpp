#include "MovingCameraBaseLayer.h"

MovingCameraBaseLayer::MovingCameraBaseLayer(unsigned int ups,
                                             const Anwill::WindowSettings& ws,
                                             float speed)
    : Anwill::Layer(ups), m_Camera(ws.width, ws.height), m_Speed(speed)
{}

void MovingCameraBaseLayer::Update(const Anwill::Timestamp& timestamp)
{
    MoveCamera();

    Layer::Update(timestamp);
}

void MovingCameraBaseLayer::MoveCamera()
{
    if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::W)) {
        m_Camera.Move(0.0f, m_Speed);
    }
    if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::A)) {
        m_Camera.Move(-m_Speed, 0.0f);
    }
    if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::S)) {
        m_Camera.Move(0.0f, -m_Speed);
    }
    if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::D)) {
        m_Camera.Move(m_Speed, 0.0f);
    }
}
