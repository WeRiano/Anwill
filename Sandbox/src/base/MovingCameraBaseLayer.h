#pragma once

#include "Anwill.h"

class MovingCameraBaseLayer : public Anwill::Layer
{
public:
    MovingCameraBaseLayer(unsigned int ups, const Anwill::WindowSettings& ws, float speed);

    void Update(const Anwill::Timestamp& timestamp) override;

protected:
    Anwill::OrthographicCamera m_Camera;
    float m_Speed;

    void MoveCamera(const Anwill::Timestamp& delta);
    void DisplayCameraGuiControls();
    float GetCameraDisplacement(const Anwill::Timestamp& timestamp) const;
};
