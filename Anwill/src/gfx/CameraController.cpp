#include "CameraController.h"
#include "events/SystemEventHandler.h"

namespace Anwill {

    OrthographicCameraController::OrthographicCameraController(float width, float height)
        : m_Camera(width, height)
    {
        SystemEventHandler::Subscribe<KeyPressEvent>(AW_BIND_THIS_MEMBER_FUNC(OnKeyPress));
    }

    void OrthographicCameraController::OnKeyPress(Unique<Event>& event)
    {
        auto& e = static_cast<KeyPressEvent&>(*event);
        switch(e.GetKeyCode()) {
            case KeyCode::W:
                m_Camera.Move({0.0f, m_Speed});
                break;
            case KeyCode::A:
                m_Camera.Move({-m_Speed, 0.0f});
                break;
            case KeyCode::S:
                m_Camera.Move({0.0f, -m_Speed});
                break;
            case KeyCode::D:
                m_Camera.Move({m_Speed, 0.0f});
                break;
        }
    }
}