#pragma once

#include "Camera.h"
#include "events/MouseEvents.h"
#include "core/Timestamp.h"

namespace Anwill {

    class OrthographicCameraController : public OrthographicCamera
    {
    public:
        OrthographicCameraController(float width, float height);

        void PollMovement(Timestamp delta);
        void ShowGuiControls();
    private:
        float m_MovementSpeed, m_ZoomSpeed;

        void OnMouseScroll(Unique<Event>& event);
    };
}
