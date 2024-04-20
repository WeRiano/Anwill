#pragma once

#include "Anwill.h"

class TextureHelloWorld : public Anwill::Layer
{
public:
    TextureHelloWorld(unsigned int ups, const Anwill::WindowSettings& ws);

    void Update(const Anwill::Timestamp& timestamp) override;

private:
    Anwill::Mesh m_Mesh;
    std::shared_ptr<Anwill::Texture> m_Texture;
    std::shared_ptr<Anwill::Shader> m_Shader;
    Anwill::OrthographicCamera m_Camera;
};
