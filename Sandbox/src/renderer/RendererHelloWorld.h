#pragma once

#include <memory>

#include "Anwill.h"

class RendererHelloWorld : public Anwill::Layer
{
public:
    RendererHelloWorld(const unsigned int ups, const Anwill::WindowSettings& ws);

    virtual void Update(const Anwill::Timestamp &timestamp) override;

private:
    std::shared_ptr<Anwill::VertexBuffer> m_VB;
    std::shared_ptr<Anwill::VertexArray> m_VA;
    std::shared_ptr<Anwill::IndexBuffer> m_IB;
    std::shared_ptr<Anwill::Shader> m_Shader;

    Anwill::OrthographicCamera m_Camera;

    Anwill::Math::Vec2f m_MousePos;
    Anwill::Mesh m_Mesh;

    void SetupLine();
    void SetupRectangle();
    void SetupTriangle();
    void SetupCheckmark();

    void OnMouseMove(std::unique_ptr<Anwill::Event>& event);
};