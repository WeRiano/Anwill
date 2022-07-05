#pragma once

#include <memory>

#include "Anwill.h"

class PhyDelta : public Anwill::Layer
{
public:
    PhyDelta(unsigned int ups, const Anwill::WindowSettings& ws);

    void Update(const Anwill::Timestamp& timestamp) override;

private:
    Anwill::Math::Vec3f m_G;
    std::shared_ptr<Anwill::Shader> m_ObjShader;
    Anwill::OrthographicCamera m_Camera;
};
