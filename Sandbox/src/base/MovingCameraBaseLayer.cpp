#include "MovingCameraBaseLayer.h"

MovingCameraBaseLayer::MovingCameraBaseLayer(unsigned int ups,
                                             const Anwill::WindowSettings& ws,
                                             float speed)
    : Anwill::Layer(ups), m_CameraController(ws.width, ws.height)
{}

void MovingCameraBaseLayer::Update(const Anwill::Timestamp& timestamp)
{
    auto delta = GetUpdateDelta(timestamp);
    //MoveCamera(delta);

    Layer::Update(timestamp);
}

void MovingCameraBaseLayer::MoveCamera(const Anwill::Timestamp& delta)
{
    float displacement = GetCameraDisplacement(delta);
    if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::W)) {
        //m_Camera.Move(0.0f, displacement);
    }
    if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::A)) {
        //m_Camera.Move(-displacement, 0.0f);
    }
    if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::S)) {
        //m_Camera.Move(0.0f, -displacement);
    }
    if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::D)) {
        //m_Camera.Move(displacement, 0.0f);
    }
}

void MovingCameraBaseLayer::DisplayCameraGuiControls()
{
    /*
    ImGui::SeparatorText("Camera");
    ImGui::Text("Position: (%.1f, %.1f)", m_Camera.GetPos().X, m_Camera.GetPos().Y);
    ImGui::SliderFloat("Speed", &m_Speed, 0.0f, 20.0f);
    if (ImGui::Button("Reset camera position")) {
        m_Camera.SetPos(0.0f, 0.0f);
    }
     */
}

float MovingCameraBaseLayer::GetCameraDisplacement(const Anwill::Timestamp& delta) const
{
    //return (float)delta.GetMilliseconds() * m_Speed;
}
