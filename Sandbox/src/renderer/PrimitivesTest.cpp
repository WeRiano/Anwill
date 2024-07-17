#include "PrimitivesTest.h"

PrimitivesTest::PrimitivesTest(const unsigned int ups, const Anwill::WindowSettings& ws)
    : Layer(ups), m_Camera(ws.width, ws.height),
    m_TriangleMesh(Anwill::Mesh::CreateTriangleMesh({-0.5f, -0.5f}, {0.5f, -0.5f}, {0.0f, 0.5f})),
    m_RectangleMesh(Anwill::Mesh::CreateRectMesh(1.0f, 1.0f)),
    m_CheckmarkMesh(Anwill::Mesh::CreateCheckmarkMesh(1.0f, 1.0f)),
    m_RectShader(Anwill::Shader::Create("Sandbox/assets/shaders/RectColor.glsl")),
    m_CircleShader(Anwill::Shader::Create("Sandbox/assets/shaders/CircleColor.glsl")),
    m_TriangleSize(ws.width / 5.0f, ws.height / 5.0f),
    m_RectangleSize(ws.width / 5.0f, ws.height / 5.0f),
    m_CircleSize(ws.width / 5.0f, ws.height / 5.0f),
    m_CheckmarkSize(ws.width / 5.0f, ws.height / 5.0f)
{
    Anwill::Renderer::SetClearColor({1.0f, 1.0f, 1.0f});
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
    m_RectShader->Unbind();

    // Rectangle
    transform = Anwill::Math::Mat4f::Scale({}, m_RectangleSize);
    transform = Anwill::Math::Mat4f::Translate(transform, {m_Camera.GetWidth() * 0.75f,
                                                           m_Camera.GetHeight() * 0.75f,
                                                           0.0f});
    m_RectShader->Bind();
    m_RectShader->SetUniformVec3f({0.8f, 0.3f, 1.0f}, "u_Color");
    Anwill::Renderer2D::SubmitMesh(m_RectShader, m_RectangleMesh, transform);
    m_RectShader->Unbind();

    // Circle
    transform = Anwill::Math::Mat4f::Scale({}, m_CircleSize);
    transform = Anwill::Math::Mat4f::Translate(transform, {m_Camera.GetWidth() * 0.25f,
                                                           m_Camera.GetHeight() * 0.25f,
                                                           0.0f});
    m_CircleShader->Bind();
    m_CircleShader->SetUniformVec3f({1.0f, 0.8f, 0.3f}, "u_Color");
    Anwill::Renderer2D::SubmitMesh(m_CircleShader, m_RectangleMesh, transform);
    m_CircleShader->Unbind();

    // Checkmark
    transform = Anwill::Math::Mat4f::Scale({}, m_CheckmarkSize);
    transform = Anwill::Math::Mat4f::Translate(transform, {m_Camera.GetWidth() * 0.75f,
                                                           m_Camera.GetHeight() * 0.25f,
                                                           0.0f});
    m_RectShader->Bind();
    m_RectShader->SetUniformVec3f({0.0f, 1.0f, 0.3f}, "u_Color");
    Anwill::Renderer2D::SubmitMesh(m_RectShader, m_CheckmarkMesh, transform);
    m_RectShader->Unbind();

    Layer::Update(timestamp);
}

void PrimitivesTest::OnDetach()
{

}
