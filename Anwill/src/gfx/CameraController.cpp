#include "CameraController.h"
#include "events/SystemEventHandler.h"
#include "imgui/Imgui.h"
#include "core/Input.h"

namespace Anwill {

    OrthographicCameraController::OrthographicCameraController(float width, float height)
        : OrthographicCamera(width, height), m_MovementSpeed(5.0f), m_ZoomSpeed(GetMaxZoom() * 0.10f)
    {
        //SystemEventHandler::Subscribe<KeyPressEvent>(AW_BIND_THIS_MEMBER_FUNC(OnKeyPress));
        //SystemEventHandler::Subscribe<KeyRepeatEvent>(AW_BIND_THIS_MEMBER_FUNC(OnKeyPress));
        SystemEventHandler::Subscribe<MouseScrollEvent>(AW_BIND_THIS_MEMBER_FUNC(OnMouseScroll));
    }

    void OrthographicCameraController::PollMovement(const Timestamp delta)
    {
        float displacement = m_MovementSpeed * (float)delta.GetMilliseconds();
        if (Input::IsKeyPressed(KeyCode::W)) {
            Move(0.0f, displacement);
        }
        if (Input::IsKeyPressed(KeyCode::A)) {
            Move(-displacement, 0.0f);
        }
        if (Input::IsKeyPressed(KeyCode::S)) {
            Move(0.0f, -displacement);
        }
        if (Input::IsKeyPressed(KeyCode::D)) {
            Move(displacement, 0.0f);
        }
    }

    void OrthographicCameraController::ShowGuiControls()
    {
        ImGui::SeparatorText("Camera");
        ImGui::Text("Position: (%.1f, %.1f)", GetPos().X, GetPos().Y);
        ImGui::Text("Zoom: %.1f", m_ZoomLevel);
        ImGui::SliderFloat("Movement speed", &m_MovementSpeed, 0.0f, 20.0f);
        ImGui::SliderFloat("Zoom speed", &m_ZoomSpeed, 0.0f, 100.0f);
        if (ImGui::Button("Reset camera position")) {
            SetPos(0.0f, 0.0f);
        }
    }

    void OrthographicCameraController::OnMouseScroll(Unique<Event>& event)
    {
        auto& e = static_cast<MouseScrollEvent&>(*event);
        switch(e.GetScrollCode()) {
            case MouseScrollCode::Up:
                Zoom(-m_ZoomSpeed);
                break;
            case MouseScrollCode::Down:
                Zoom(m_ZoomSpeed);
                break;
        }
    }
}
