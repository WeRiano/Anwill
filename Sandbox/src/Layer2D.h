#pragma once

#include <memory>

#include "Anwill.h"

class Layer2D : public Anwill::Layer
{
public:
    Layer2D(const unsigned int ups);

    virtual void Update(const Anwill::Timestamp &timestamp) override;

private:
    std::shared_ptr<Anwill::VertexBuffer> m_VB;
    std::shared_ptr<Anwill::VertexArray> m_VA;
    std::shared_ptr<Anwill::IndexBuffer> m_IB;
    std::shared_ptr<Anwill::Shader> m_Shader;

    Anwill::OrthographicCamera m_Camera;
};