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
    Anwill::Mesh m_TriangleMesh, m_RectangleMesh, m_CheckmarkMesh;
    Anwill::Shared<Anwill::Shader> m_RectShader, m_CircleShader;
    Anwill::Math::Vec2f m_TriangleSize, m_RectangleSize, m_CircleSize, m_CheckmarkSize;
};