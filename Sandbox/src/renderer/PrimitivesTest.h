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

    Anwill::Mesh m_TriangleMesh, m_RectangleMesh, m_CheckmarkMesh;
    Anwill::Shared<Anwill::Shader> m_RectShader, m_CircleShader;
    const Anwill::Math::Vec2f m_WindowSize;
    Anwill::Math::Vec2f m_TriangleSize, m_RectangleSize, m_CircleSize, m_CheckmarkSize,
                        m_TrianglePos, m_RectanglePos, m_CirclePos, m_CheckmarkPos;
    Anwill::Math::Vec3f m_BackgroundColor, m_TriangleColor, m_RectangleColor, m_CircleColor, m_CheckmarkColor;
};