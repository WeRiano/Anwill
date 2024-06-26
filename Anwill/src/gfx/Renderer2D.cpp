#include "gfx/Renderer2D.h"
#include "gfx/ShaderMacros.h"
#include "utils/Utils.h"
#include "utils/Profiler.h"

namespace Anwill {

    std::shared_ptr<GraphicsAPI> Renderer2D::s_API = nullptr;
    Renderer2D::SceneData2D Renderer2D::s_SceneData;
    QuadBatchData Renderer2D::s_QData;
    CircleBatchData Renderer2D::s_CData;

    void Renderer2D::Init(const std::shared_ptr<GraphicsAPI>& api)
    {
        s_API = api;
        BatchDataInit();
        //Font::s_Shader = Shader::Create("Anwill/res/shaders/OpenGL/Font.glsl");
        Mesh::Init();
    }

    void Renderer2D::BeginScene(const Camera& camera)
    {
        s_SceneData = SceneData2D();
        s_SceneData.ViewProjMat = camera.GetViewProj();
        s_SceneData.CameraPos = camera.GetPos();
    }

    void Renderer2D::PushQuadToBatch(const Math::Mat4f& transform,
                                     const Math::Vec3f& color)
    {
        if(s_QData.elementsPushed == QuadBatchData::maxQuads) {
            DrawQuadBatch();
        }
        s_QData.VertexToArr({transform * QuadBatchData::unitPositions[0], {},
                             color}, -1);
        s_QData.VertexToArr({transform * QuadBatchData::unitPositions[1], {},
                             color}, -1);
        s_QData.VertexToArr({transform * QuadBatchData::unitPositions[2], {},
                             color}, -1);
        s_QData.VertexToArr({transform * QuadBatchData::unitPositions[3], {},
                             color}, -1);
        s_QData.elementsPushed++;
    }

    void Renderer2D::PushQuadToBatch(const Math::Mat4f& transform,
                                     const Sprite& sprite)
    {
        if((s_QData.textureQ.size() >= BatchData2D::maxTextureSlots) or
           (s_QData.elementsPushed >= QuadBatchData::maxQuads)) {
            DrawQuadBatch();
        }
        unsigned int textureID = s_QData.GetOrGenerateID(sprite.texture);

        s_QData.VertexToArr({transform * QuadBatchData::unitPositions[0],
                             Math::Vec2f(sprite.texCoords.x0, sprite.texCoords.y0),
                             {1.0f, 1.0f, 1.0f}},
                            textureID);
        s_QData.VertexToArr({transform * QuadBatchData::unitPositions[1],
                             Math::Vec2f(sprite.texCoords.x0, sprite.texCoords.y1),
                             {1.0f, 1.0f, 1.0f}},
                            textureID);
        s_QData.VertexToArr({transform * QuadBatchData::unitPositions[2],
                             Math::Vec2f(sprite.texCoords.x1, sprite.texCoords.y1),
                             {1.0f, 1.0f, 1.0f}},
                            textureID);
        s_QData.VertexToArr({transform * QuadBatchData::unitPositions[3],
                             Math::Vec2f(sprite.texCoords.x1, sprite.texCoords.y0),
                             {1.0f, 1.0f, 1.0f}},
                            textureID);
        s_QData.elementsPushed++;
    }

    void Renderer2D::PushQuadToBatch(const Math::Mat4f& transform,
                                     const SpriteAnimation& animation)
    {
        if(animation.IsEmpty())
        {
            AW_WARN("Pushed empty animation to batch renderer");
            return;
        }
        auto sprite = animation.GetActiveFrame();
        PushQuadToBatch(transform, sprite);
    }

    void Renderer2D::PushCircleToBatch(const Math::Mat4f& transform,
                                       const Math::Vec3f& color)
    {
        if(s_CData.elementsPushed == CircleBatchData::maxCircles) {
            DrawCircleBatch();
        }

        auto scaleVec = transform.GetScale();
        float xRadius = scaleVec.X / 2;
        float yRadius = scaleVec.Y / 2;
        auto centre = (transform * Math::Vec2f()) + s_SceneData.CameraPos;
        s_CData.VertexToArr({transform * QuadBatchData::unitPositions[0], {},
                             color, centre, Math::Vec2f(xRadius, yRadius)});
        s_CData.VertexToArr({transform * QuadBatchData::unitPositions[1], {},
                             color, centre, Math::Vec2f(xRadius, yRadius)});
        s_CData.VertexToArr({transform * QuadBatchData::unitPositions[2], {},
                             color, centre, Math::Vec2f(xRadius, yRadius)});
        s_CData.VertexToArr({transform * QuadBatchData::unitPositions[3], {},
                             color, centre, Math::Vec2f(xRadius, yRadius)});
        s_CData.elementsPushed++;
    }

    void Renderer2D::DrawBatch()
    {
        AW_PROFILE_FUNC();
        DrawQuadBatch();
        DrawCircleBatch();
        //DrawLineBatch(); TODO
    }

    void Renderer2D::SubmitText(const std::shared_ptr<Shader>& shader, Font& font,
                                const std::string& text, const Math::Mat4f& transform,
                                float maxWidth)
    {
        shader->Bind();
        shader->SetUniformMat4f(transform, "u_Transform");
        shader->SetUniformMat4f(s_SceneData.ViewProjMat, "u_ViewProjMat");

        Math::Vec2f batchStartPos = {};
        auto maxTextSlots = *ShaderMacros::GetMacro<unsigned int>("AW_MAX_FRAGMENT_SAMPLERS");
        std::string remainingText = text; // Everything is remaining at the start
        while (!remainingText.empty()) {
            auto thisStr = Utils::UniqueCharsSubstr(remainingText, maxTextSlots);
            // Prepare and draw the text
            batchStartPos = font.Prepare(thisStr, shader, batchStartPos, maxWidth);
            s_API->DrawFont(font, thisStr);
            font.Done();
            // Grab the next batch
            remainingText = remainingText.substr(thisStr.size());
        }

        shader->Unbind();
    }

    void Renderer2D::SubmitVertices(const std::shared_ptr<Shader>& shader,
                                    const std::shared_ptr<VertexArray>& vertexArray,
                                    const std::shared_ptr<IndexBuffer>& indexBuffer,
                                    const Math::Mat4f& transform)
    {
        shader->Bind();
        shader->SetUniformMat4f(transform, "u_Transform");
        shader->SetUniformMat4f(s_SceneData.ViewProjMat, "u_ViewProjMat");

        s_API->DrawIndexed(vertexArray, indexBuffer);

        shader->Unbind();
    }

    void Renderer2D::SubmitMesh(const std::shared_ptr<Shader> &shader,
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

        s_API->DrawMesh(mesh);

        if(texture != nullptr) {
            texture->Unbind();
        }

        shader->Unbind();
    }

    void Renderer2D::SubmitMesh(const std::shared_ptr<Shader>& shader,
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

        s_API->DrawMesh(mesh);

        textures[textures.size()-1]->Unbind();
        shader->Unbind();
    }

    void Renderer2D::SubmitSprite(const std::shared_ptr<Shader>& shader, const Sprite& sprite,
                                  const Math::Mat4f& transform)
    {
        // TODO
        // Makes no sense right now to submit a sprite, because vertex array has to be created render-time
        // (with texCoords). Sprite should have one that we can just grab, but that is not necessary when its used
        // for batch renderer.

        shader->Bind();
        shader->SetUniformMat4f(transform, "u_Transform");
        shader->SetUniformMat4f(s_SceneData.ViewProjMat, "u_ViewProjMat");

        sprite.texture->Bind();

        s_API->DrawMesh(sprite.GetQuadMesh());

        sprite.texture->Unbind();

        shader->Unbind();
    }

    void Renderer2D::SubmitLines(const std::shared_ptr<Shader>& shader,
                                 const std::shared_ptr<VertexArray>& vertexArray,
                                 const Math::Mat4f transform,
                                 unsigned int linesCount)
    {
        shader->Bind();
        shader->SetUniformMat4f(transform, "u_Transform");
        shader->SetUniformMat4f(s_SceneData.ViewProjMat, "u_ViewProjMat");

        if(linesCount == 1) {
            s_API->DrawLine(vertexArray);
        } else {
            s_API->DrawLines(vertexArray, linesCount);
        }

        shader->Unbind();
    }

    void Renderer2D::BatchDataInit()
    {
        // Runtime constant
        BatchData2D::maxTextureSlots = *ShaderMacros::GetMacro<unsigned int>
                ("AW_MAX_FRAGMENT_SAMPLERS");

        // Runtime objects / variables
        // Vertex buffer
        s_QData.VB = VertexBuffer::Create(sizeof(float) *
                                          QuadBatchData::verticesArrMaxSize);
        s_CData.VB = VertexBuffer::Create(sizeof(float) *
                                          CircleBatchData::verticesArrMaxSize);
        s_QData.elementsPushed = 0;
        s_CData.elementsPushed = 0;
        s_QData.verticesArrIndex = 0;
        s_CData.verticesArrIndex = 0;
        s_QData.verticesArr = new float[QuadBatchData::verticesArrMaxSize];
        s_CData.verticesArr = new float[CircleBatchData::verticesArrMaxSize];

        // Index buffer
        // Use the same index buffer for circles and quads
        s_QData.IB = IndexBuffer::Create(QuadBatchData::indicesMaxElementSize);
        s_QData.indicesArr = new unsigned int[QuadBatchData::indicesMaxElementSize];
        unsigned int max = std::max<unsigned int>(QuadBatchData::maxQuads,
                                    std::max<unsigned int>(CircleBatchData::maxCircles,
                                                           0));
        for(unsigned int i = 0; i < max; i++)
        {
            s_QData.indicesArr[i * 6 + 0] = (i * 4) + 0;
            s_QData.indicesArr[i * 6 + 1] = (i * 4) + 1;
            s_QData.indicesArr[i * 6 + 2] = (i * 4) + 2;

            s_QData.indicesArr[i * 6 + 3] = (i * 4) + 0;
            s_QData.indicesArr[i * 6 + 4] = (i * 4) + 2;
            s_QData.indicesArr[i * 6 + 5] = (i * 4) + 3;
        }

        // Vertex Array
        s_QData.VA = VertexArray::Create();
        auto qLayout = BufferLayout({
            BufferElement(ShaderDataType::Float2),
            BufferElement(ShaderDataType::Float2),
            BufferElement(ShaderDataType::Float3),
            BufferElement(ShaderDataType::Float)
        });
        s_QData.VA->AddBuffer(*s_QData.VB, qLayout);

        s_CData.VA = VertexArray::Create();
        auto cLayout = BufferLayout({
           BufferElement(ShaderDataType::Float2),
           BufferElement(ShaderDataType::Float2),
           BufferElement(ShaderDataType::Float3),
           BufferElement(ShaderDataType::Float2),
           BufferElement(ShaderDataType::Float2)
        });
        s_CData.VA->AddBuffer(*s_CData.VB, cLayout);

        // Shaders
        s_QData.shader = s_API->CreateQuadBatchShader();
        s_CData.shader = s_API->CreateCircleBatchShader();

        s_QData.textureNewIDCount = BatchData2D::startTextureID;
    }

    void Renderer2D::DrawQuadBatch()
    {
        if(!s_QData.elementsPushed) {
            return;
        }
        s_QData.shader->Bind();
        s_QData.shader->SetUniformMat4f(s_SceneData.ViewProjMat, "u_ViewProjMat");

        unsigned int nrTextures = s_QData.textureQ.size();
        for (int textID = 0; textID < nrTextures; textID++)
        {
            const auto& texture = s_QData.textureQ.front();
            s_QData.textureQ.pop();
            texture->Bind(textID);
            s_QData.shader->SetUniform1i(textID, "u_Textures[" + std::to_string(textID) + "]");
        }

        s_QData.VB->DynamicUpdate(s_QData.verticesArr,
                                  sizeof(float) * s_QData.elementsPushed *
                                  QuadBatchData::quadAttribCount);
        s_QData.IB->DynamicUpdate(s_QData.indicesArr,
                                  s_QData.elementsPushed * 6);
        s_API->DrawIndexed(s_QData.VA, s_QData.IB);

        s_QData.elementsPushed = 0;
        s_QData.verticesArrIndex = 0;
        s_QData.textureNewIDCount = BatchData2D::startTextureID;
        s_QData.textureMap.clear();
        s_QData.shader->Unbind();
    }

    void Renderer2D::DrawCircleBatch()
    {
        if(!s_CData.elementsPushed) {
            return;
        }
        s_CData.shader->Bind();
        s_CData.shader->SetUniformMat4f(s_SceneData.ViewProjMat, "u_ViewProjMat");

        // Reusing quad index buffer since they are identical
        s_CData.VB->DynamicUpdate(s_CData.verticesArr,
                                  sizeof(float) * s_CData.elementsPushed *
                                  CircleBatchData::circleAttribCount);
        s_QData.IB->DynamicUpdate(s_QData.indicesArr,
                                  s_CData.elementsPushed * 6);
        s_API->DrawIndexed(s_CData.VA, s_QData.IB);

        s_CData.elementsPushed = 0;
        s_CData.verticesArrIndex = 0;
        s_CData.shader->Unbind();
    }
}