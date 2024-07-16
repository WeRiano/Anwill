#include "PrimitivesTest.h"

PrimitivesTest::PrimitivesTest(const unsigned int ups, const Anwill::WindowSettings& ws)
    : Layer(ups), m_Camera(ws.width, ws.height),
    m_TriangleMesh(Anwill::Mesh::CreateTriangleMesh({-0.5f, -0.5f}, {0.5f, -0.5f}, {0.0f, 0.5f})),
    m_RectangleMesh(Anwill::Mesh::CreateRectMesh(1.0f, 1.0f)),
    m_RectShader(Anwill::Shader::Create("Sandbox/assets/shaders/RectColor.glsl")),
    m_TriangleSize(ws.width / 5.0f, ws.height / 5.0f)
{
    Anwill::Renderer::SetClearColor({1.0f, 1.0f, 1.0f});

    //Anwill::SystemEventHandler::Subscribe<Anwill::MouseMoveEvent>(AW_BIND_THIS_MEMBER_FUNC(OnMouseMove));
    m_GuiWindow = Anwill::Gui::CreateWindow("Primitives test");
    Anwill::Gui::Text("Triangle");
    Anwill::Gui::Slider(10.0f, ws.width / 4.0f, m_TriangleSize.X, false);
    Anwill::Gui::Slider(10.0f, ws.height / 4.0f, m_TriangleSize.Y, false);
}

void PrimitivesTest::Update(const Anwill::Timestamp& timestamp)
{
    Anwill::Renderer2D::BeginScene(m_Camera);

    // Triangle
    auto transform = Anwill::Math::Mat4f::Scale({}, m_TriangleSize);
    transform = Anwill::Math::Mat4f::Translate(transform, {m_Camera.GetWidth() * 0.25f,
                                                           m_Camera.GetHeight() * 0.75f,
                                                           0.0f});
    m_RectShader->Bind();
    m_RectShader->SetUniformVec3f({0.3f, 0.8f, 1.0f}, "u_Color");
    Anwill::Renderer2D::SubmitMesh(m_RectShader, m_TriangleMesh, transform);

    // Rectangle
    // TODO

    Layer::Update(timestamp);
}

void PrimitivesTest::OnDetach()
{
    Anwill::Gui::EraseWindow(m_GuiWindow);
}
