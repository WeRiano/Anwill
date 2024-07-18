#include "FontTest.h"

FontTest::FontTest(unsigned int ups, const Anwill::WindowSettings& ws)
    : MovingCameraBaseLayer(ups, ws, 5.0f),
      m_Font(Anwill::Font("Sandbox/assets/fonts/arial.ttf")),
      m_Shader(Anwill::Shader::Create("Anwill/res/shaders/OpenGL/Font.glsl")),
      m_FontPos(ws.width * 0.5f, ws.height * 0.5f)
{}

void FontTest::Update(const Anwill::Timestamp& timestamp)
{
    Anwill::Renderer2D::BeginScene(m_Camera);

    static int fontSize = 40;

    ImGui::Begin("Font");

    ImGui::SliderInt("Size", &fontSize, 10, 300);

    DisplayCameraGuiControls();

    ImGui::End();

    auto scale = m_Font.GetScaleValue(fontSize);
    auto transform = Anwill::Math::Mat4f::Scale(Anwill::Math::Mat4f::Identity(),
                                                {scale, scale, 0.0f});
    transform = Anwill::Math::Mat4f::Translate(transform, m_FontPos);
    transform = Anwill::Math::Mat4f::Translate(transform, {-40.0f, 0.0f, 0.0f});
    Anwill::Renderer2D::SubmitText(m_Shader, m_Font,
                                   "ABCDEFGHIJKLMNOPQRSTUVWXYZ\n"
                                   "1234567890abcdefghijklmnopqrstuvxyz\n",
                                   //"\t[](){}!@£#¤$%€&/=\\+?<>|,.-;:_-",
                                   transform);

    MovingCameraBaseLayer::Update(timestamp);
}
