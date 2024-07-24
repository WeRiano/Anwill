#pragma once

#include "Anwill.h"

class ArenaRender : public Anwill::Layer
{
public:
    ArenaRender(unsigned int ups, const Anwill::WindowSettings& ws);

    void Update(const Anwill::Timestamp& timestamp) override;
private:
    Anwill::OrthographicCamera m_Camera;
    std::shared_ptr<Anwill::Shader> m_RectShader, m_CircleShader;
};
