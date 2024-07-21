#pragma once

#include "Anwill.h"

class MovingCameraBaseLayer : public Anwill::Layer
{
public:
    MovingCameraBaseLayer(unsigned int ups, const Anwill::WindowSettings& ws, float speed);

    void Update(const Anwill::Timestamp& timestamp) override;

protected:
    Anwill::OrthographicCameraController m_CameraController;
};
