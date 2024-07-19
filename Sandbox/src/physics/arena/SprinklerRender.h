#pragma once

#include <memory>

#include "Anwill.h"

class SprinklerRender : public Anwill::Layer
{
public:
    SprinklerRender(unsigned int ups, const Anwill::WindowSettings& ws);

    void Update(const Anwill::Timestamp& timestamp) override;

private:
    Anwill::OrthographicCamera m_Camera;
};
