#include "gfx/Renderer2D.h"
#include "gfx/ShaderMacros.h"
#include "utils/Utils.h"
#include "utils/Profiler.h"

namespace Anwill {

    std::shared_ptr<GraphicsAPI> Renderer2D::s_API = nullptr;
    Renderer2D::SceneData2D Renderer2D::s_SceneData;
    BatchData2D Renderer2D::s_BData;

    void Renderer2D::Init(const std::shared_ptr<GraphicsAPI>& api)
    {
        s_API = api;
        BatchDataInit();
    }

    void Renderer2D::DrawBatch(const std::shared_ptr<Shader>& shader)
    {
        AW_PROFILE_FUNC();

        shader->Bind();
        shader->SetUniformMat4f(s_SceneData.ViewProjMat, "u_ViewProjMat");

        unsigned int nrTextures = s_BData.textureQ.size();
        for (int textID = 0; textID < nrTextures; textID++)
        {
            const auto& texture = s_BData.textureQ.front();
            s_BData.textureQ.pop();
            texture->Bind(textID);
            shader->SetUniform1i(textID, "u_Textures[" + std::to_string(textID) + "]");
        }

        s_BData.quadVB->DynamicUpdate(s_BData.quadVerticesArr,
                                      sizeof(float) * s_BData.quadsPushed *
                                      BatchData2D::quadElementCount);
        s_BData.quadIB->DynamicUpdate(s_BData.quadIndices,
                                      s_BData.quadsPushed * 6);
        s_API->Draw(s_BData.quadVA, s_BData.quadIB);

        s_BData.quadsPushed = 0;
        s_BData.quadVerticesArrIndex = 0;
        s_BData.textureNewIDCount = BatchData2D::startTextureID;
        s_BData.textureMap.clear();
        shader->Unbind();
    }

    void Renderer2D::PushQuadToBatch(const Math::Mat4f& transform,
                                     const std::shared_ptr<Shader>& shader,
                                     const Math::Vec3f& color)
    {
        if(s_BData.quadsPushed == BatchData2D::maxQuads) {
            DrawBatch(shader);
        }
        s_BData.QuadVertexToArr({transform * BatchData2D::baselineQuadPositions[0], {},
                                 color}, -1);
        s_BData.QuadVertexToArr({transform * BatchData2D::baselineQuadPositions[1], {},
                                 color}, -1);
        s_BData.QuadVertexToArr({transform * BatchData2D::baselineQuadPositions[2], {},
                                 color}, -1);
        s_BData.QuadVertexToArr({transform * BatchData2D::baselineQuadPositions[3], {},
                                 color}, -1);
        s_BData.quadsPushed++;
    }

    void Renderer2D::PushQuadToBatch(const Math::Mat4f& transform,
                                     const std::shared_ptr<Shader>& shader,
                                     const Sprite& sprite)
    {
        if((s_BData.textureQ.size() == BatchData2D::maxTextureSlots) or
           (s_BData.quadsPushed == BatchData2D::maxQuads)) {
            DrawBatch(shader);
        }
        unsigned int textureID = s_BData.GetOrGenerateID(sprite.texture);
        s_BData.QuadVertexToArr({transform * BatchData2D::baselineQuadPositions[0],
                                 Math::Vec2f(sprite.texCoords.x0, sprite.texCoords.y0),
                                 {1.0f, 1.0f, 1.0f}},
                                textureID);
        s_BData.QuadVertexToArr({transform * BatchData2D::baselineQuadPositions[1],
                                 Math::Vec2f(sprite.texCoords.x0, sprite.texCoords.y1),
                                 {1.0f, 1.0f, 1.0f}},
                                textureID);
        s_BData.QuadVertexToArr({transform * BatchData2D::baselineQuadPositions[2],
                                 Math::Vec2f(sprite.texCoords.x1, sprite.texCoords.y1),
                                 {1.0f, 1.0f, 1.0f}},
                                textureID);
        s_BData.QuadVertexToArr({transform * BatchData2D::baselineQuadPositions[3],
                                 Math::Vec2f(sprite.texCoords.x1, sprite.texCoords.y0),
                                 {1.0f, 1.0f, 1.0f}},
                                textureID);
        s_BData.quadsPushed++;
    }

    void Renderer2D::PushQuadToBatch(const Math::Mat4f& transform,
                                     const std::shared_ptr<Shader>& shader,
                                     const SpriteAnimation& animation)
    {
        std::shared_ptr<Texture> texture;
        QuadTexCoords texCoords;
        auto sprite = animation.GetActiveFrame();
        PushQuadToBatch(transform, shader, sprite);
    }

    void Renderer2D::BeginScene(const Camera& camera)
    {
        s_SceneData = SceneData2D();
        s_SceneData.ViewProjMat = camera.GetViewProj();
    }

    void Renderer2D::Submit(const std::shared_ptr<Shader>& shader, Font& font,
                          const std::string& text, const Math::Mat4f& transform)
    {
        shader->Bind();
        shader->SetUniformMat4f(transform, "u_Transform");
        shader->SetUniformMat4f(s_SceneData.ViewProjMat, "u_ViewProjMat");

        int batchStartXPos = 0;
        auto maxTextSlots = *ShaderMacros::GetMacro<unsigned int>("AW_MAX_TEXTURE_SLOTS");
        std::string remainingText = text; // Everything is remaining at the start
        while (!remainingText.empty()) {
            auto thisStr = Utils::UniqueCharsSubstr(remainingText, maxTextSlots);
            // Prepare and draw the text
            batchStartXPos = font.Prepare(thisStr, shader, batchStartXPos);
            s_API->Draw(font, text);
            // Grab the next batch
            remainingText = remainingText.substr(thisStr.size());
        }

        font.Done();
        shader->Unbind();
    }

    void Renderer2D::Submit(const std::shared_ptr<Shader>& shader,
                          const std::shared_ptr<VertexArray>& vertexArray,
                          const std::shared_ptr<IndexBuffer>& indexBuffer,
                          const Math::Mat4f& transform)
    {
        shader->Bind();
        shader->SetUniformMat4f(transform, "u_Transform");
        shader->SetUniformMat4f(s_SceneData.ViewProjMat, "u_ViewProjMat");

        s_API->Draw(vertexArray, indexBuffer);

        shader->Unbind();
    }

    void Renderer2D::Submit(const std::shared_ptr<Shader> &shader,
                          const Mesh& mesh,
                          const Math::Mat4f& transform,
                          const std::shared_ptr<Texture>& texture)
    {
        shader->Bind();
        shader->SetUniformMat4f(transform, "u_Transform");
        shader->SetUniformMat4f(s_SceneData.ViewProjMat, "u_ViewProjMat");
        if(texture != nullptr) {
            shader->SetUniform1i(0, "u_TextureSampler");
            texture->Bind();
        }

        s_API->Draw(mesh);

        if(texture != nullptr) {
            texture->Unbind();
        }
        shader->Unbind();
    }

    void Renderer2D::Submit(const std::shared_ptr<Shader>& shader,
                          const Mesh& mesh,
                          const Math::Mat4f& transform,
                          const std::vector<std::shared_ptr<Texture>>& textures)
    {
        shader->Bind();
        shader->SetUniformMat4f(transform, "u_Transform");
        shader->SetUniformMat4f(s_SceneData.ViewProjMat, "u_ViewProjMat");
        for(unsigned int i = 0; i < textures.size(); i++)
        {
            const auto& texture = textures[i];
            if (texture != nullptr)
            {
                texture->Bind(i);
                shader->SetUniform1i(i, "u_TextureSampler");
            }
        }

        s_API->Draw(mesh);

        textures[textures.size()-1]->Unbind();
        shader->Unbind();
    }

    void Renderer2D::BatchDataInit()
    {
        // Runtime constant
        BatchData2D::maxTextureSlots = *ShaderMacros::GetMacro<unsigned int>
                ("AW_MAX_TEXTURE_SLOTS");

        // Runtime objects / variables
        // Vertex buffer
        s_BData.quadVB = VertexBuffer::Create(sizeof(float) *
                                              BatchData2D::quadVerticesArrMaxSize);
        s_BData.quadsPushed = 0;
        s_BData.quadVerticesArrIndex = 0;

        // Index buffer
        s_BData.quadIB = IndexBuffer::Create(BatchData2D::quadIndicesMaxElementSize);
        s_BData.quadIndices = new unsigned int[BatchData2D::quadIndicesMaxElementSize];
        for(unsigned int i = 0; i < BatchData2D::maxQuads; i++)
        {
            s_BData.quadIndices[i * 6 + 0] = (i * 4) + 0;
            s_BData.quadIndices[i * 6 + 1] = (i * 4) + 1;
            s_BData.quadIndices[i * 6 + 2] = (i * 4) + 2;

            s_BData.quadIndices[i * 6 + 3] = (i * 4) + 0;
            s_BData.quadIndices[i * 6 + 4] = (i * 4) + 2;
            s_BData.quadIndices[i * 6 + 5] = (i * 4) + 3;
        }

        // Vertex Array
        s_BData.quadVA = VertexArray::Create();
        auto layout = BufferLayout({
            BufferElement(ShaderDataType::Float2),
            BufferElement(ShaderDataType::Float2),
            BufferElement(ShaderDataType::Float3),
            BufferElement(ShaderDataType::Float)
        });
        s_BData.quadVA->AddBuffer(*s_BData.quadVB, layout);

        s_BData.textureNewIDCount = BatchData2D::startTextureID;
    }
}