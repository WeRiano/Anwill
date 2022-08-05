#include "FontHelloWorld.h"

FontHelloWorld::FontHelloWorld(unsigned int ups, const Anwill::WindowSettings& ws)
    : Anwill::Layer(ups), m_Font(Anwill::Font("assets/fonts/arial.ttf")),
      m_Camera(ws.width, ws.height)
{
    m_Shader = Anwill::Shader::Create("assets/shaders/Font.glsl");
    float cameraMoveX = (float) ws.width / 2.0f;
    float cameraMoveY = (float) ws.height / 2.0f;
    m_Camera.Move(-cameraMoveX, -cameraMoveY);
}

void FontHelloWorld::Update(const Anwill::Timestamp& timestamp)
{
    Anwill::Renderer::BeginScene(m_Camera);

    auto transform = Anwill::Math::Mat4f::Scale(Anwill::Math::Mat4f::Identity(),
                                                {1.0f, 1.0f, 0.0f});
    transform = Anwill::Math::Mat4f::Translate(transform, {0.0f, 0.0f, 0.0f});
    Anwill::Renderer::Submit(m_Shader, m_Font,
                             "ABCDEFGHIJKLMNOPQRSTUVXYZ1234567890!?+-`\"<>=", transform);
    MoveCamera();

    Layer::Update(timestamp);
}

void FontHelloWorld::MoveCamera()
{
    float speed = 1.0f;
    if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::W)) {
        m_Camera.Move(0.0f, speed);
    }
    if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::A)) {
        m_Camera.Move(-speed, 0.0f);
    }
    if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::S)) {
        m_Camera.Move(0.0f, -speed);
    }
    if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::D)) {
        m_Camera.Move(speed, 0.0f);
    }
}
