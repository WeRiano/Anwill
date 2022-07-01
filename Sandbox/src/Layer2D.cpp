#include "Layer2D.h"

Layer2D::Layer2D(const unsigned int ups)
    : Layer(ups), m_Camera(800.0f, 600.0f)
{
    float vertices[] = {
        500.0f, 500.0f,  // top right
        500.0f, 100.0f,  // bottom right
        100.0f, 100.0f,  // bottom left
        100.0f, 500.0f  // top left
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    m_VB = Anwill::VertexBuffer::Create(vertices, sizeof(vertices));
    m_IB = Anwill::IndexBuffer::Create(indices, 6);

    auto elements = {
           Anwill::BufferElement(Anwill::ShaderDataType::Float2)
    };

    auto bufferLayout = Anwill::BufferLayout(elements);
    m_VA = Anwill::VertexArray::Create();
    m_VA->AddBuffer(*m_VB.get(), bufferLayout);

    m_Shader = Anwill::Shader::Create("assets/shaders/HelloUniform.glsl");

    Anwill::Renderer::SetClearColor({0.1f, 0.1f, 0.1f});
}

void Layer2D::Update(const Anwill::Timestamp& timestamp)
{
    Layer::Update(timestamp);

    Anwill::Renderer::BeginScene(m_Camera);

    m_Camera.Move(0.001f, 0.001f);
    Anwill::Renderer::Submit(m_Shader, m_VA, m_IB, Anwill::Math::Mat4f::Identity());
}
