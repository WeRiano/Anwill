#include "TextureTest.h"

TextureTest::TextureTest(unsigned int ups, const Anwill::WindowSettings& ws)
    : MovingCameraBaseLayer(ups, ws, 5.0f),
      m_Mesh(Anwill::Mesh::CreateRectMesh(1.0f, 1.0f, true)),
      m_Texture(Anwill::Texture::Create("Sandbox/assets/textures/awesomeface.png")),
      m_Shader(Anwill::Shader::Create("Sandbox/assets/shaders/RectTexture.glsl")),
      m_TextureSize(ws.width, ws.height),
      m_TexturePos(ws.width / 2.0f, ws.height / 2.0f)

{
    Anwill::Renderer::SetClearColor({0.0f, 0.0f, 0.0f});
}

void TextureTest::Update(const Anwill::Timestamp& timestamp)
{
    ImGui::Begin("Texture");

    ImGui::SliderFloat2("Texture size", &m_TextureSize.X, 10.0f, 5000.0f);

    DisplayCameraGuiControls();

    ImGui::End();

    Anwill::Renderer2D::BeginScene(m_Camera);

    auto transform = Anwill::Math::Mat4f::Scale({}, m_TextureSize);
    transform = Anwill::Math::Mat4f::Translate(transform, m_TexturePos);
    Anwill::Renderer2D::SubmitMesh(m_Shader, m_Mesh, transform, m_Texture);

    MovingCameraBaseLayer::Update(timestamp);
}
