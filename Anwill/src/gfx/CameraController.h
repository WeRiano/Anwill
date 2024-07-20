#pragma once

#include "Camera.h"
#include "events/KeyEvents.h"

namespace Anwill {

    class CameraController
    {

    };

    class OrthographicCameraController : public CameraController
    {
    public:
        OrthographicCameraController(float width, float height);
    private:
        OrthographicCamera m_Camera;
        float m_Speed;

        void OnKeyPress(Unique<Event>& event);
    };
}
