#include "TextureHelloWorld.h"

TextureHelloWorld::TextureHelloWorld(unsigned int ups, const Anwill::WindowSettings& ws)
    : Anwill::Layer(ups), m_Camera(ws.width, ws.height)
{
    m_Texture = Anwill::Texture::Create("assets/textures/awesomeface.png");
    m_Shader = Anwill::Shader::Create("assets/shaders/RectTexture.glsl");
    m_Mesh = Anwill::Mesh::CreateRectMesh(ws.width, ws.height, true);
    float cameraMoveX = (float) ws.width / 2.0f;
    float cameraMoveY = (float) ws.height / 2.0f;
    m_Camera.Move(-cameraMoveX, -cameraMoveY);

    //Anwill::Renderer::SetClearColor({0.3f, 0.1f, 0.05f});
}

void TextureHelloWorld::Update(const Anwill::Timestamp& timestamp)
{
    Anwill::Renderer::BeginScene(m_Camera);

    Anwill::Renderer::Submit(m_Shader, m_Mesh, Anwill::Math::Mat4f::Identity(), m_Texture);

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

    Layer::Update(timestamp);
}
