#include "RendererHelloWorld"

RendererHelloWorld::RendererHelloWorld(const unsigned int ups)
    : Layer(ups), m_Camera(800.0f, 600.0f)
{
    //SetupLine();
    //SetupRectangle();
    this->SetupTriangle();
}

void RendererHelloWorld::Update(const Anwill::Timestamp& timestamp)
{
    Anwill::Renderer2D::BeginScene(m_Camera);

    Anwill::Renderer2D::SubmitVertices(m_Shader, m_VA, m_IB, Anwill::Math::Mat4f::Identity());
    //Anwill::Renderer2D::SubmitLines(m_Shader, m_VA, Anwill::Math::Mat4f::Identity(), 1);

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
    m_Shader->SetUniformVec3f({1.0f, 1.0f, 1.0f}, "u_Color");
}
