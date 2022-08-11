#include "GraphicsContext.h"
#include "Renderer.h"
#include "ShaderMacros.h"
#include "utils/Utils.h"

#undef min

namespace Anwill {

    unsigned int BatchData2D::maxTextureSlots;
    const std::array<Math::Vec2f, 4> BatchData2D::baselineQuadPositions = {
            Math::Vec2f{-0.5f, -0.5f},
            Math::Vec2f{-0.5f,  0.5f},
            Math::Vec2f{ 0.5f,  0.5f},
            Math::Vec2f{ 0.5f, -0.5f}
    };

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
        shader->Bind();
        shader->SetUniformMat4f(s_SceneData.ViewProjMat, "u_ViewProjMat");

        // Just remove the first texture-less batch if it is empty
        bool textLessBatch = !s_B2Data.quadsByTexture[0].empty();
        if(!textLessBatch) {
            s_B2Data.quadsByTexture.erase(s_B2Data.quadsByTexture.begin());
        }
        while(!s_B2Data.quadsByTexture.empty())
        {
            // We either go until empty or until we hit maximum nr of texture slots avail
            // Each quad batch has the same texture (or is texture-less)
            unsigned int quadBatchesToRender = std::min<unsigned int>(
                    s_B2Data.quadsByTexture.size(),
                    BatchData2D::maxTextureSlots + textLessBatch);

            // Find out how many quads we draw this batch
            unsigned int quadsToDraw = 0;
            for(unsigned int i = 0; i < quadBatchesToRender; i++)
            {
                quadsToDraw += s_B2Data.quadsByTexture[i].size();
            }

            unsigned int arrOffset = 0;
            // Form native array with all the vertices that we then feed to VB
            for (unsigned int i = 0; i < quadBatchesToRender; i++)
            {
                // Again, all vertices in this batch have the same texture
                auto quadBatch = s_B2Data.quadsByTexture[i];
                for(const auto& quad : quadBatch)
                {
                    s_B2Data.QuadToArr(quad, i - textLessBatch, arrOffset);
                    arrOffset += BatchData2D::quadElementCount;
                }
            }
            s_B2Data.quadVB->DynamicUpdate(s_B2Data.quadVerticesArr,
                                           sizeof(float) * quadsToDraw *
                                           BatchData2D::quadElementCount);
            s_B2Data.quadIB->DynamicUpdate(s_B2Data.quadIndices,
                                           quadsToDraw * 6);

            // Bind a texture of each quad batch that we are rendering
            // Get quickly from FIFO queue which their id is implied
            for (int textID = 0; textID < quadBatchesToRender - textLessBatch; textID++)
            {
                const auto& texture = s_B2Data.textureQ.front();
                s_B2Data.textureQ.pop();
                texture->Bind(textID);
                shader->SetUniform1i(textID,
                                     "u_Textures[" + std::to_string(textID) + "]");
            }

            s_API->Draw(s_B2Data.quadVA, s_B2Data.quadIB);

            // Delete all the quads that we drew from dynamic storage
            auto eraseEndIterVec = s_B2Data.quadsByTexture.begin();
            std::advance(eraseEndIterVec, quadBatchesToRender);
            s_B2Data.quadsByTexture.erase(s_B2Data.quadsByTexture.begin(),
                                          eraseEndIterVec);
            // After first draw call we are always done with the texture-less batch
            textLessBatch = false;
        }
        s_B2Data.quadsPushed = 0;
        s_B2Data.textureNewIDCount = BatchData2D::startTextureID;
        s_B2Data.textureMap.clear();
        // Put back the index for texture-less quads
        s_B2Data.quadsByTexture.emplace_back();
        shader->Unbind();
    }

    void Renderer::PushQuadToBatch(const Math::Mat4f& transform, const Math::Vec3f& color)
    {
        s_B2Data.quadsByTexture[0].emplace_back(
                QuadVertex(transform * BatchData2D::baselineQuadPositions[0], {}, color),
                QuadVertex(transform * BatchData2D::baselineQuadPositions[1], {}, color),
                QuadVertex(transform * BatchData2D::baselineQuadPositions[2], {}, color),
                QuadVertex(transform * BatchData2D::baselineQuadPositions[3], {}, color));
        s_B2Data.quadsPushed++;
    }

    void Renderer::PushQuadToBatch(const Math::Mat4f& transform,
                                   const std::shared_ptr<Texture>& texture,
                                   float texX0, float texY0, float texX1, float texY1)
    {
        unsigned int textureID = s_B2Data.GetOrGenerateID(texture);
        s_B2Data.quadsByTexture[textureID].emplace_back(
                QuadVertex(transform * BatchData2D::baselineQuadPositions[0],
                           Math::Vec2f(texX0, texY0), {1.0f, 1.0f, 1.0f}),
                QuadVertex(transform * BatchData2D::baselineQuadPositions[1],
                           Math::Vec2f(texX0, texY1), {1.0f, 1.0f, 1.0f}),
                QuadVertex(transform * BatchData2D::baselineQuadPositions[2],
                           Math::Vec2f(texX1, texY1), {1.0f, 1.0f, 1.0f}),
                QuadVertex(transform * BatchData2D::baselineQuadPositions[3],
                           Math::Vec2f(texX1, texY0), {1.0f, 1.0f, 1.0f}));
        s_B2Data.quadsPushed++;
    }

    void Renderer::PushQuadToBatch(const Math::Mat4f& transform,
                                   const std::shared_ptr<SpriteSheet>& spriteSheet,
                                   unsigned int spriteXPos, unsigned int spriteYPos)
    {
        // Same as texture version (see function above) but we find specific tex coords
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
        // Runtime constant
        BatchData2D::maxTextureSlots = *ShaderMacros::GetMacro<unsigned int>
                ("AW_MAX_TEXTURE_SLOTS");
        // Texture-less quads have the first index reserved.
        s_B2Data.quadsByTexture.emplace_back();

        // Runtime objects / variables
            // Vertex buffer
        s_B2Data.quadVB = VertexBuffer::Create(sizeof(float) *
                BatchData2D::quadVerticesArrMaxSize);
        s_B2Data.quadsPushed = 0;

            // Index buffer
        s_B2Data.quadIB = IndexBuffer::Create(BatchData2D::quadIndicesMaxElementSize);
        s_B2Data.quadIndices = new unsigned int[BatchData2D::quadIndicesMaxElementSize];
        for(unsigned int i = 0; i < BatchData2D::maxQuads; i++)
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
            BufferElement(ShaderDataType::Float3),
            BufferElement(ShaderDataType::Float)
        });
        s_B2Data.quadVA->AddBuffer(*s_B2Data.quadVB, layout);

        s_B2Data.textureNewIDCount = BatchData2D::startTextureID;
    }
}