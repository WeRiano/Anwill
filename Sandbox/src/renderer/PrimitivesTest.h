#pragma once

#include <memory>

#include "Anwill.h"

class PrimitivesTest : public Anwill::Layer
{
public:
    PrimitivesTest(const unsigned int ups, const Anwill::WindowSettings& ws);

    virtual void Update(const Anwill::Timestamp &timestamp) override;
    virtual void OnDetach() override;

private:
    Anwill::OrthographicCamera m_Camera;

    //Anwill::Math::Vec2f m_MousePos;
    Anwill::Mesh m_TriangleMesh, m_RectangleMesh;
    Anwill::Shared<Anwill::Shader> m_RectShader;
    Anwill::Math::Vec2f m_TriangleSize, m_RectangleSize, m_CircleSize, m_CheckmarkSize;
    Anwill::Shared<Anwill::GuiWindow> m_GuiWindow;
};