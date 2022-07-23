#pragma once

#include "Anwill.h"

class FontHelloWorld : public Anwill::Layer
{
public:
    FontHelloWorld(unsigned int ups, const Anwill::WindowSettings& ws);

    void Update(const Anwill::Timestamp& timestamp) override;

private:
    Anwill::Font m_Font;
    std::shared_ptr<Anwill::Shader> m_Shader;
    Anwill::OrthographicCamera m_Camera;

    void MoveCamera();
};
