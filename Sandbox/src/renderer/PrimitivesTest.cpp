#include "PrimitivesTest.h"

PrimitivesTest::PrimitivesTest(const unsigned int ups, const Anwill::WindowSettings& ws)
    : Layer(ups), m_Camera(ws.width, ws.height),
    m_TriangleMesh(Anwill::Mesh::CreateTriangleMesh({-0.5f, -0.5f}, {0.5f, -0.5f}, {0.0f, 0.5f})),
    m_RectangleMesh(Anwill::Mesh::CreateRectMesh(1.0f, 1.0f)),
    m_CheckmarkMesh(Anwill::Mesh::CreateCheckmarkMesh(1.0f, 1.0f)),
    m_RectShader(Anwill::Shader::Create("Sandbox/assets/shaders/RectColor.glsl")),
    m_CircleShader(Anwill::Shader::Create("Sandbox/assets/shaders/CircleColor.glsl")),
    m_TriangleSize(ws.width / 5.0f, ws.width / 5.0f),
    m_RectangleSize(ws.width / 5.0f, ws.width / 5.0f),
    m_CircleSize(ws.width / 5.0f, ws.width / 5.0f),
    m_CheckmarkSize(ws.width / 5.0f, ws.width / 5.0f),
    m_WindowSize(ws.width, ws.height),
    m_TrianglePos(-(float)ws.width * 0.25f, (float)ws.height * 0.25f),
    m_RectanglePos((float)ws.width * 0.25f, (float)ws.height * 0.25f),
    m_CirclePos((float)ws.width * 0.25f, -(float)ws.height * 0.25f),
    m_CheckmarkPos(-(float)ws.width * 0.25f, -(float)ws.height * 0.25f),
    m_BackgroundColor(1.0f, 1.0f, 1.0f),
    m_TriangleColor(0.3f, 0.8f, 1.0f),
    m_RectangleColor(0.8f, 0.3f, 1.0f),
    m_CircleColor(1.0f, 0.8f, 0.3f),
    m_CheckmarkColor(0.0f, 1.0f, 0.3f)
{}

void PrimitivesTest::Update(const Anwill::Timestamp& timestamp)
{
    Anwill::Renderer2D::BeginScene(m_Camera);

    Anwill::Renderer::SetClearColor(m_BackgroundColor);

    float maxPos = m_WindowSize.Max() * 0.5f;

    ImGui::Begin("Primitives");

    ImGui::PushID(0);
    ImGui::SeparatorText("Background");
    ImGui::ColorEdit3("Color", &m_BackgroundColor.X);
    ImGui::PopID();

    ImGui::SeparatorText("Triangle");
    ImGui::PushID(1);
    ImGui::SliderFloat2("Scale", &m_TriangleSize.X, 10.0f, 1000.0f);
    ImGui::SliderFloat2("Translation", &m_TrianglePos.X, -maxPos, maxPos);
    ImGui::ColorEdit3("Color", &m_TriangleColor.X);
    ImGui::PopID();

    ImGui::SeparatorText("Rectangle");
    ImGui::PushID(2);
    ImGui::SliderFloat2("Scale", &m_RectangleSize.X, 10.0f, 1000.0f);
    ImGui::SliderFloat2("Translation", &m_RectanglePos.X, -maxPos, maxPos);
    ImGui::ColorEdit3("Color", &m_RectangleColor.X);
    ImGui::PopID();

    ImGui::SeparatorText("Circle");
    ImGui::PushID(3);
    ImGui::SliderFloat2("Scale", &m_CircleSize.X, 10.0f, 1000.0f);
    ImGui::SliderFloat2("Translation", &m_CirclePos.X, -maxPos, maxPos);
    ImGui::ColorEdit3("Color", &m_CircleColor.X);
    ImGui::PopID();

    ImGui::SeparatorText("Checkmark");
    ImGui::PushID(4);
    ImGui::SliderFloat2("Scale", &m_CheckmarkSize.X, 10.0f, 1000.0f);
    ImGui::SliderFloat2("Translation", &m_CheckmarkPos.X, -maxPos, maxPos);
    ImGui::ColorEdit3("Color", &m_CheckmarkColor.X);
    ImGui::PopID();

    ImGui::End();

    // Triangle
    auto transform = Anwill::Math::Mat4f::Scale({}, m_TriangleSize);
    transform = Anwill::Math::Mat4f::Translate(transform, m_TrianglePos);
    m_RectShader->Bind();
    m_RectShader->SetUniformVec3f(m_TriangleColor, "u_Color");
    Anwill::Renderer2D::SubmitMesh(m_RectShader, m_TriangleMesh, transform);
    m_RectShader->Unbind();

    // Rectangle
    transform = Anwill::Math::Mat4f::Scale({}, m_RectangleSize);
    transform = Anwill::Math::Mat4f::Translate(transform, m_RectanglePos);
    m_RectShader->Bind();
    m_RectShader->SetUniformVec3f(m_RectangleColor, "u_Color");
    Anwill::Renderer2D::SubmitMesh(m_RectShader, m_RectangleMesh, transform);
    m_RectShader->Unbind();

    // Circle
    transform = Anwill::Math::Mat4f::Scale({}, m_CircleSize);
    transform = Anwill::Math::Mat4f::Translate(transform, m_CirclePos);
    m_CircleShader->Bind();
    m_CircleShader->SetUniformVec3f(m_CircleColor, "u_Color");
    Anwill::Renderer2D::SubmitMesh(m_CircleShader, m_RectangleMesh, transform);
    m_CircleShader->Unbind();

    // Checkmark
    transform = Anwill::Math::Mat4f::Scale({}, m_CheckmarkSize);
    transform = Anwill::Math::Mat4f::Translate(transform, m_CheckmarkPos);
    m_RectShader->Bind();
    m_RectShader->SetUniformVec3f(m_CheckmarkColor, "u_Color");
    Anwill::Renderer2D::SubmitMesh(m_RectShader, m_CheckmarkMesh, transform);
    m_RectShader->Unbind();

    Layer::Update(timestamp);
}

void PrimitivesTest::OnDetach()
{

}
