#pragma once

#include "base/MovingCameraBaseLayer.h"
#include "Anwill.h"

class FontTest : public MovingCameraBaseLayer
{
public:
    FontTest(unsigned int ups, const Anwill::WindowSettings& ws);

    void Update(const Anwill::Timestamp& timestamp) override;

private:
    Anwill::Font m_Font;
    std::shared_ptr<Anwill::Shader> m_Shader;
    const Anwill::Math::Vec2f m_FontPos;
};
