#pragma once

#include <memory>
#include <string>
#include <functional>
#include <vector>

#include "events/Event.h"
#include "math/Mat4f.h"
#include "gfx/Font.h"
#include "gfx/Mesh.h"
#include "gfx/Renderer2D.h"

namespace Anwill {

    typedef unsigned int GuiWindowID;

    struct GuiElement {
        static Mesh s_RectMesh;
        static std::unique_ptr<Font> s_Font;

        Math::Mat4f transform;

        virtual void Render() = 0;
    };

    template <typename... Args>
    struct GuiButton : public GuiElement {
        std::string text;
        std::function<void(Args&...)> callback;
    };

    struct GuiDropdown : public GuiElement {
        std::vector<std::shared_ptr<GuiElement>> elements;
    };

    struct GuiWindow {
        static std::shared_ptr<Shader> s_WindowShader;

        GuiWindowID id;
        std::string title;
        Math::Mat4f transform;
        std::vector<std::shared_ptr<GuiElement>> elements;

        void Render(bool selected) {
            RenderWindow(selected);
            for (unsigned int i = 0; i < elements.size(); i++) {
                elements[i]->Render();
            }
        }

    private:
        void RenderWindow(bool selected) {
            auto windowScale = transform.GetScale();
            s_WindowShader->Bind();
            s_WindowShader->SetUniformVec2f({windowScale.GetX(), windowScale.GetY()},
                                            "u_Size");
            s_WindowShader->SetUniform1i(selected, "u_Selected");
            Renderer2D::Submit(s_WindowShader, GuiElement::s_RectMesh, transform);
            auto textScale = Font::GetScaleValue(12);
            auto textTransform = Math::Mat4f::Scale(Math::Mat4f::Identity(),
                                                    {textScale, textScale, 0.0f});
            Math::Vec2f titlePos = GetTitleStartPos(title);
            textTransform = Math::Mat4f::Translate(textTransform,
                                                   {titlePos.GetX(), titlePos.GetY(),
                                                    0.0f});
            Renderer2D::Submit(Font::s_Shader, *GuiElement::s_Font, title, textTransform);
        }

        Math::Vec2f GetTitleStartPos(const std::string& titleStr) {
            float borderSize = 8.0f;
            float headerSize = borderSize * 2.5f;
            float margin = 5.0f;

            float xMax, yMax, yMin;
            // TODO: Hide if larger than window width
            GuiElement::s_Font->GetTextSize(titleStr, xMax, yMax, yMin);
            auto windowScale = transform.GetScale();
            float width = windowScale.GetX();
            float height = windowScale.GetY();
            Math::Vec3f origin = transform.GetTranslateVector();
            return {origin.GetX() - width / 2 + borderSize * 2 + margin,
                    origin.GetY() + height / 2 - headerSize + margin};
        }
    };

    class Gui
    {
    public:
        static void Init(const WindowSettings& ws);
        static void Render();

        template <class E, typename... Args>
        static void AddElement(GuiWindowID windowID, Args&&... args) {
            if(windowID == 0) {
                s_Windows[0].elements.emplace_back(E(std::forward<Args>(args)...));
            }

            for(unsigned int i = 0; i < s_Windows.size(); i++) {
                if (s_Windows[i].id == windowID) {
                    s_Windows[0].elements.emplace_back(E(std::forward<Args>(args)...));
                    break;
                }
            }
        }

        static GuiWindowID NewWindow(const std::string& title);

    private:
        static std::unique_ptr<OrthographicCamera> s_Camera;
        static std::vector<GuiWindow> s_Windows;
        static GuiWindowID s_NextID;

        static Math::Vec2f s_MousePos;
        static bool s_Moving;
        static bool s_ScalingX, s_ScalingY;

        static void OnMouseMove(std::unique_ptr<Event>& event);
        static void OnMousePress(std::unique_ptr<Event>& event);
        static void OnMouseRelease(std::unique_ptr<Event>& event);
    };
}
