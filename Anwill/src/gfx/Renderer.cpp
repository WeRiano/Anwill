#include "GraphicsContext.h"
#include "Renderer.h"
#include "ShaderMacros.h"
#include "utils/Utils.h"

namespace Anwill {

    GraphicsAPI::API Renderer::s_APIName;
    std::unique_ptr<GraphicsAPI> Renderer::s_API;
    Renderer::SceneData Renderer::s_SceneData;
    BatchData2D Renderer::s_B2Data;

    void Renderer::Init()
    {
        s_API = GraphicsAPI::Create(s_APIName);

        BatchDataInit();
    }

    GraphicsAPI::API Renderer::GetAPI()
    {
        return s_APIName;
    }

    void Renderer::SetAPI(GraphicsAPI::API api)
    {
        s_APIName = api;
    }

    void Renderer::SetViewport(unsigned int x, unsigned int y,
                               unsigned int width, unsigned int height)
    {
        s_API->SetViewport(x, y, width, height);
    }

    void Renderer::SetClearColor(const Math::Vec3f& color)
    {
        s_API->SetClearColor(color);
    }

    void Renderer::ClearBuffers()
    {
        s_API->ClearBuffers();
    }

    void Renderer::DrawBatch(const std::shared_ptr<Shader>& shader)
    {
        s_B2Data.quadVB->DynamicUpdate(s_B2Data.quadVertices,
                                       sizeof(float) * s_B2Data.quadVertexCount * 4 *
                                       s_B2Data.quadsPushed);
        s_B2Data.quadIB = IndexBuffer::Create(s_B2Data.quadIndices,
                                              s_B2Data.quadsPushed * 6);

        shader->Bind();
        //shader->SetUniformMat4f(transform, "u_Transform");
        shader->SetUniformMat4f(s_SceneData.ViewProjMat, "u_ViewProjMat");
        for(auto i = s_B2Data.textureMap.begin(); i != s_B2Data.textureMap.end(); i++)
        {
            auto texture = i->first;
            auto id = i->second;
            texture->Bind(id);
            //shader->SetUniform1i(id, "u_Textures[" + std::to_string(id) + "]");
        }
        s_API->Draw(s_B2Data.quadVA, s_B2Data.quadIB);
        shader->Unbind();

        s_B2Data.quadsPushed = 0;
        s_B2Data.quadVerticesIndex = 0;
        s_B2Data.textureMap.clear();
        s_B2Data.textureNewIDCount = 0;
    }

    void Renderer::PushQuadToBatch(const Math::Mat4f& transform,
                                   const std::shared_ptr<Texture>& texture,
                                   float texX0, float texY0, float texX1, float texY1)
    {
        unsigned int offset = s_B2Data.quadVerticesIndex;
        // Going clockwise
        Math::Vec2f p0 = transform * Math::Vec2f{-0.5f, -0.5f};
        Math::Vec2f p1 = transform * Math::Vec2f{-0.5f,  0.5f};
        Math::Vec2f p2 = transform * Math::Vec2f{ 0.5f,  0.5f};
        Math::Vec2f p3 = transform * Math::Vec2f{ 0.5f, -0.5f};

        unsigned int textureID = s_B2Data.GetOrGenerateID(texture);

        s_B2Data.quadVertices[offset + 0] = p0.GetX();
        s_B2Data.quadVertices[offset + 1] = p0.GetY();
        s_B2Data.quadVertices[offset + 2] = texX0;
        s_B2Data.quadVertices[offset + 3] = texY0;
        s_B2Data.quadVertices[offset + 4] = static_cast<float>(textureID);

        s_B2Data.quadVertices[offset + 5] = p1.GetX();
        s_B2Data.quadVertices[offset + 6] = p1.GetY();
        s_B2Data.quadVertices[offset + 7] = texX0;
        s_B2Data.quadVertices[offset + 8] = texY1;
        s_B2Data.quadVertices[offset + 9] = static_cast<float>(textureID);

        s_B2Data.quadVertices[offset + 10] = p2.GetX();
        s_B2Data.quadVertices[offset + 11] = p2.GetY();
        s_B2Data.quadVertices[offset + 12] = texX1;
        s_B2Data.quadVertices[offset + 13] = texY1;
        s_B2Data.quadVertices[offset + 14] = static_cast<float>(textureID);

        s_B2Data.quadVertices[offset + 15] = p3.GetX();
        s_B2Data.quadVertices[offset + 16] = p3.GetY();
        s_B2Data.quadVertices[offset + 17] = texX1;
        s_B2Data.quadVertices[offset + 18] = texY0;
        s_B2Data.quadVertices[offset + 19] = static_cast<float>(textureID);

        s_B2Data.quadsPushed++;
        s_B2Data.quadVerticesIndex += s_B2Data.quadVertexCount * 4;
    }

    void Renderer::PushQuadToBatch(const Math::Mat4f& transform,
                                   const std::shared_ptr<SpriteSheet>& spriteSheet,
                                   unsigned int spriteXPos, unsigned int spriteYPos)
    {
        float texX0, texY0, texX1, texY1;
        spriteSheet->GetEvenSpriteTexCoords(spriteXPos, spriteYPos, texX0, texY0,
                                            texX1, texY1);
        PushQuadToBatch(transform, spriteSheet->GetTexture(), texX0, texY0,
                         texX1, texY1);
    }

    void Renderer::BeginScene(const Camera& camera)
    {
        s_SceneData = SceneData();
        s_SceneData.ViewProjMat = camera.GetViewProj();
    }

    void Renderer::Submit(const std::shared_ptr<Shader>& shader, Font& font,
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

    void Renderer::Submit(const std::shared_ptr<Shader>& shader,
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

    void Renderer::Submit(const std::shared_ptr<Shader> &shader,
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

    void Renderer::Submit(const std::shared_ptr<Shader>& shader,
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

    void Renderer::BatchDataInit()
    {
        // Runtime constants
        s_B2Data.maxQuads = 20000;
        s_B2Data.maxTextureSlots = *ShaderMacros::GetMacro<unsigned int>
                ("AW_MAX_TEXTURE_SLOTS");
        // 2 pos, 2 tex coord, 1 texture
        s_B2Data.quadVertexCount = 5;
        // 4 vertices per quad when we use an index buffer
        s_B2Data.quadVerticesMaxSize = s_B2Data.quadVertexCount * 4 * s_B2Data.maxQuads;

        // Runtime objects / variables that will be altered
            // Vertex buffer
        s_B2Data.quadVB = VertexBuffer::Create(sizeof(float) *
                s_B2Data.quadVerticesMaxSize);
        s_B2Data.quadVertices = new float[s_B2Data.quadVerticesMaxSize];
        s_B2Data.quadsPushed = 0;
        s_B2Data.quadVerticesIndex = 0;

            // Index buffer
        const unsigned int indicesMaxSize = s_B2Data.maxQuads * 6;
        s_B2Data.quadIndices = new unsigned int[indicesMaxSize];
        for(unsigned int i = 0; i < s_B2Data.maxQuads; i++)
        {
            s_B2Data.quadIndices[i * 6 + 0] = (i * 4) + 0;
            s_B2Data.quadIndices[i * 6 + 1] = (i * 4) + 1;
            s_B2Data.quadIndices[i * 6 + 2] = (i * 4) + 2;

            s_B2Data.quadIndices[i * 6 + 3] = (i * 4) + 0;
            s_B2Data.quadIndices[i * 6 + 4] = (i * 4) + 2;
            s_B2Data.quadIndices[i * 6 + 5] = (i * 4) + 3;
        }

            // Vertex Array
        s_B2Data.quadVA = VertexArray::Create();
        auto layout = BufferLayout({
            BufferElement(ShaderDataType::Float2),
            BufferElement(ShaderDataType::Float2),
            BufferElement(ShaderDataType::Float)
        });
        s_B2Data.quadVA->AddBuffer(*s_B2Data.quadVB, layout);

        // Shader
        //s_B2Data.quadShader = Shader::Create(); // TODO:

        s_B2Data.textureNewIDCount = 0;
    }
}