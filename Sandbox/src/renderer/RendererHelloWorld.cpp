#include "RendererHelloWorld.h"

RendererHelloWorld::RendererHelloWorld(const unsigned int ups, const Anwill::WindowSettings& ws)
    : Layer(ups), m_Camera(ws.width, ws.height), m_MousePos()
{
    Anwill::Renderer::SetClearColor({1.0f, 1.0f, 1.0f});
    //SetupLine();
    //SetupRectangle();
    //SetupTriangle();
    SetupCheckmark();

    Anwill::SystemEventHandler::Subscribe<Anwill::MouseMoveEvent>(AW_BIND_THIS_MEMBER_FUNC(OnMouseMove));
}

void RendererHelloWorld::Update(const Anwill::Timestamp& timestamp)
{
    Anwill::Renderer2D::BeginScene(m_Camera);

    float width = m_MousePos.X;
    float height = m_MousePos.X;
    auto transform = Anwill::Math::Mat4f::Scale({}, {width, height, 0.0f});
    transform = Anwill::Math::Mat4f::Translate(transform, {m_Camera.GetWidth() * 0.5f,
                                                           m_Camera.GetHeight() * 0.5f,
                                                           0.0f});
    //Anwill::Renderer2D::SubmitVertices(m_Shader, m_VA, m_IB, transform);
    //Anwill::Renderer2D::SubmitLines(m_Shader, m_VA, Anwill::Math::Mat4f::Identity(), 1);
    Anwill::Renderer2D::SubmitMesh(m_Shader, m_Mesh, transform);

    Layer::Update(timestamp);
}

void RendererHelloWorld::SetupLine()
{
    float vertices[] = {
        500.0f, 500.0f,  // top right
        500.0f, 100.0f  // bottom right
    };

    m_VB = Anwill::VertexBuffer::Create(vertices, sizeof(vertices));

    std::vector<Anwill::BufferElement> elements = {
        Anwill::BufferElement(Anwill::ShaderDataType::Float2)
    };

    auto bufferLayout = Anwill::BufferLayout(elements);
    m_VA = Anwill::VertexArray::Create();
    m_VA->AddBuffer(*m_VB.get(), bufferLayout);

    //m_Shader = Anwill::Shader::Create("Sandbox/assets/shaders/RectBorder.glsl");
    m_Shader = Anwill::Shader::Create("Sandbox/assets/shaders/RectColor.glsl");
    m_Shader->Bind();
    m_Shader->SetUniformVec3f({1.0f, 1.0f, 1.0f}, "u_Color");

    Anwill::Renderer::SetClearColor({0.1f, 0.1f, 0.1f});
}

void RendererHelloWorld::SetupRectangle()
{
    float vertices[] = {
        0.5f, 0.5f,  // top right
        0.5f, -0.5f,  // bottom right
        -0.5f, -0.5f,  // bottom left
        -0.5f, 0.5f  // top left
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    m_VB = Anwill::VertexBuffer::Create(vertices, sizeof(vertices));
    m_IB = Anwill::IndexBuffer::Create(indices, 6);

    std::vector<Anwill::BufferElement> elements = {
        Anwill::BufferElement(Anwill::ShaderDataType::Float2)
    };

    auto bufferLayout = Anwill::BufferLayout(elements);
    m_VA = Anwill::VertexArray::Create();
    m_VA->AddBuffer(*m_VB.get(), bufferLayout);

    //m_Shader = Anwill::Shader::Create("Sandbox/assets/shaders/RectBorder.glsl");
    //m_Shader = Anwill::Shader::Create("Sandbox/assets/shaders/RectColor.glsl");
    m_Shader = Anwill::Shader::Create("Sandbox/assets/shaders/CircleColor.glsl");
    m_Shader->Bind();
    m_Shader->SetUniformVec3f({0.3f, 0.8f, 1.0f}, "u_Color");
}

void RendererHelloWorld::SetupTriangle()
{
    float vertices[] = {
        300.0f, 500.0f,  // top
        500.0f, 100.0f,  // bottom right
        100.0f, 100.0f,  // bottom left
    };

    unsigned int indices[] = {
        0, 1, 3
    };

    m_VB = Anwill::VertexBuffer::Create(vertices, sizeof(vertices));
    m_IB = Anwill::IndexBuffer::Create(indices, 3);

    std::vector<Anwill::BufferElement> elements = {
        Anwill::BufferElement(Anwill::ShaderDataType::Float2)
    };

    auto bufferLayout = Anwill::BufferLayout(elements);
    m_VA = Anwill::VertexArray::Create();
    m_VA->AddBuffer(*m_VB.get(), bufferLayout);

    //m_Shader = Anwill::Shader::Create("Sandbox/assets/shaders/RectBorder.glsl");
    m_Shader = Anwill::Shader::Create("Sandbox/assets/shaders/RectColor.glsl");
    m_Shader->Bind();
    m_Shader->SetUniformVec3f({0.3f, 0.8f, 1.0f}, "u_Color");
}

void RendererHelloWorld::SetupCheckmark()
{
    m_Mesh = Anwill::Mesh::CreateCheckmarkMesh(1.0f, 1.0f);
    m_Shader = Anwill::Shader::Create("Sandbox/assets/shaders/RectColor.glsl");
    m_Shader->Bind();
    m_Shader->SetUniformVec3f({0.3f, 0.8f, 1.0f}, "u_Color");
}

void RendererHelloWorld::OnMouseMove(std::unique_ptr<Anwill::Event>& event)
{
    auto e = static_cast<Anwill::MouseMoveEvent&>(*event);
    m_MousePos = {e.GetXPos(), e.GetYPos()};
}
