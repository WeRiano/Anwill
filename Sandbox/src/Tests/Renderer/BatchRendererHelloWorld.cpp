#include "BatchRendererHelloWorld.h"
#include "utils/FileIO.h"

BatchRendererHelloWorld::BatchRendererHelloWorld(unsigned int ups,
                                                 const Anwill::WindowSettings& ws)
    : Anwill::Layer(ups), m_Camera(ws.width, ws.height)
{
    m_QuadShader = Anwill::Shader::Create("assets/shaders/QuadBatch.glsl");
    m_TestSpriteSheet = Anwill::SpriteSheet::Create(
            "assets/textures/test_sprite_sheet.png", 64, 48);
    m_TestTexture1 = Anwill::Texture::Create("assets/textures/awesomeface.png");
    m_TestTexture2 = Anwill::Texture::Create("assets/textures/awesomeface.png");
    m_TestTexture3 = Anwill::Texture::Create("assets/textures/awesomeface.png");

    //Anwill::FileIO::StrToFile();
}

void BatchRendererHelloWorld::Update(const Anwill::Timestamp& timestamp)
{
    Anwill::Renderer::BeginScene(m_Camera);

    auto transform = Anwill::Math::Mat4f::Scale(Anwill::Math::Mat4f::Identity(),
                                                {40.0f, 40.0, 0.0f});
    transform = Anwill::Math::Mat4f::Translate(transform, {20.0f, 20.0f, 0.0f});
    unsigned int textureRow = 1, textureCol = 1;
    unsigned int rowSize = 40;
    for(unsigned int i = 0; i < 1000; i++)
    {
        if(i != 0)
        {
            if (i % rowSize == 0)
            {
                transform = Anwill::Math::Mat4f::Translate(transform,
                                                           {-60.0f * (rowSize - 1.0f),
                                                            40.0f, 0.0f});
                textureRow++;
                textureCol = 1;
            } else
            {
                transform = Anwill::Math::Mat4f::Translate(transform,
                                                           {60.0f, 0.0f, 0.0f});
                textureCol++;
            }
        }
        Anwill::Renderer::PushQuadToBatch(transform, m_TestSpriteSheet,
                                          textureCol + 10 % 64, textureRow + 10 % 48);
    }

    auto transform2 = Anwill::Math::Mat4f::Scale(Anwill::Math::Mat4f::Identity(),
                                                {40.0f, 40.0, 0.0f});
    transform2 = Anwill::Math::Mat4f::Translate(transform2,
                                                {-200.0f, 0.0f, 0.0f});
    Anwill::Renderer::PushQuadToBatch(transform2, m_TestTexture1);
    transform2 = Anwill::Math::Mat4f::Translate(transform2,
                                                {-200.0f, 0.0f, 0.0f});
    Anwill::Renderer::PushQuadToBatch(transform2, m_TestTexture2);
    transform2 = Anwill::Math::Mat4f::Translate(transform2,
                                                {-200.0f, 0.0f, 0.0f});
    Anwill::Renderer::PushQuadToBatch(transform2, m_TestTexture3);

    Anwill::Renderer::DrawBatch(m_QuadShader);

    MoveCamera();

    Layer::Update(timestamp);
}

void BatchRendererHelloWorld::MoveCamera()
{
    float speed = 0.5f;
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