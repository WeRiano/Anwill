#pragma once

#include <memory>

#include "Anwill.h"

class DynamicsRender : public Anwill::Layer
{
public:
    DynamicsRender(unsigned int ups, const Anwill::WindowSettings& ws);

    void Update(const Anwill::Timestamp& timestamp) override;

private:
    std::shared_ptr<Anwill::Shader> m_ObjShader;
    Anwill::OrthographicCamera m_Camera;
};
