#include "GraphicsContext.h"
#include "Renderer.h"
#include "ShaderMacros.h"
#include "utils/Utils.h"

#include <iostream>

namespace Anwill {

    GraphicsAPI::API Renderer::s_APIName;
    std::unique_ptr<GraphicsAPI> Renderer::s_API;
    Renderer::SceneData Renderer::s_SceneData;

    void Renderer::Init()
    {
        s_API = GraphicsAPI::Create(s_APIName);
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

        std::string remainingText = text; // Everything is remaining at the start
        while (!remainingText.empty()) {
            auto thisStr = Utils::UniqueCharsSubstr(remainingText,
                        *ShaderMacros::GetMacro<unsigned int>("AW_MAX_TEXTURE_SLOTS"));
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
}