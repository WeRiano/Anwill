#pragma once

#include "Anwill.h"

class GuiTest : public Anwill::Layer
{
public:
    GuiTest(const unsigned int ups);

    virtual void Update(const Anwill::Timestamp &timestamp) override;

private:
    std::shared_ptr<Anwill::VertexBuffer> m_VB;
    std::shared_ptr<Anwill::VertexArray> m_VA;
    std::shared_ptr<Anwill::IndexBuffer> m_IB;
    std::shared_ptr<Anwill::Shader> m_Shader;

    Anwill::OrthographicCamera m_Camera;
};
