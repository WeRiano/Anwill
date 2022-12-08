#pragma once

#include <memory>
#include <string>
#include <functional>
#include <vector>

#include "events/Event.h"
#include "math/Mat4f.h"
#include "math/Algo.h"
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

        bool IsHoveringHeader(const Math::Vec2f& mousePos) {
            auto origin = transform.GetTranslateVector();
            auto size = transform.GetScale();
            return (Math::Algo::IsPointInsideRectangle({origin.GetX() - size.GetX() / 2,
                                                   origin.GetY() + size.GetY() / 2},
                                                  {origin.GetX() + size.GetX() / 2,
                                                   origin.GetY() + size.GetY() / 2},
                                                  {origin.GetX() + size.GetX() / 2,
                                                   origin.GetY() + size.GetY() / 2
                                                   - s_HeaderSize},
                                                  {origin.GetX() - size.GetX() / 2,
                                                   origin.GetY() + size.GetY() / 2
                                                   - s_HeaderSize},
                                                  mousePos));
        }

        bool IsHoveringResize(const Math::Vec2f& mousePos) {
            auto origin = transform.GetTranslateVector();
            auto size = transform.GetScale();
            return Math::Algo::IsPointInsideCircle({origin.GetX() + size.GetX() / 2,
                                             origin.GetY() - size.GetY() / 2},
                                            10.0f,
                                            mousePos);
        }

        bool IsHoveringWindow(const Math::Vec2f& mousePos) {
            auto origin = transform.GetTranslateVector();
            auto size = transform.GetScale();
            return Math::Algo::IsPointInsideRectangle({origin.GetX() - size.GetX() / 2,
                                                origin.GetY() + size.GetY() / 2},
                                               {origin.GetX() + size.GetX() / 2,
                                                origin.GetY() + size.GetY() / 2},
                                               {origin.GetX() + size.GetX() / 2,
                                                origin.GetY() - size.GetY() / 2},
                                               {origin.GetX() - size.GetX() / 2,
                                                origin.GetY() - size.GetY() / 2},
                                               mousePos);
        }

    private:
        static constexpr float s_BorderSize = 8.0f;
        static constexpr float s_HeaderSize = s_BorderSize * 2.5f;
        static constexpr float s_Margin = 5.0f;

        void RenderWindow(bool selected) {
            // Render Window
            auto windowScale = transform.GetScale();
            s_WindowShader->Bind();
            s_WindowShader->SetUniformVec2f({windowScale.GetX(), windowScale.GetY()}, "u_Size");
            s_WindowShader->SetUniform1i(selected, "u_Selected");
            Renderer2D::Submit(s_WindowShader, GuiElement::s_RectMesh, transform);

            // Set Text Size to 12
            auto textScale = Font::GetScaleValue(12);
            auto textTransform = Math::Mat4f::Scale(Math::Mat4f::Identity(),
                                                    {textScale, textScale, 0.0f});

            // Get the largest title string that we can fit inside header
            unsigned int textMaxWidth = transform.GetScale().GetX() - (s_BorderSize * 2 + s_Margin) * 2;
            std::string parsedTitle = GuiElement::s_Font->GetLargestSubstr(title, textMaxWidth * 1.0f / textScale);

            // Get the position of the title
            Math::Vec2f titleStartPos = GetTitleStartPos();
            textTransform = Math::Mat4f::Translate(textTransform, {titleStartPos.GetX(), titleStartPos.GetY(), 0.0f});

            // Render title
            Renderer2D::Submit(Font::s_Shader, *GuiElement::s_Font, parsedTitle, textTransform);
        }

        Math::Vec2f GetTitleStartPos() {
            auto windowSize = transform.GetScale();
            float width = windowSize.GetX();
            float height = windowSize.GetY();
            Math::Vec3f origin = transform.GetTranslateVector();
            return {origin.GetX() - width / 2 + s_BorderSize * 2 + s_Margin,
                     origin.GetY() + height / 2 - s_HeaderSize + s_Margin};
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
        static bool s_Moving, s_ScalingX, s_ScalingY;
        static bool s_HoveringDiagonalScaling, s_HoveringHeader;
        static unsigned int s_HoveringWindow;

        static void OnMouseMove(std::unique_ptr<Event>& event);
        static void OnMousePress(std::unique_ptr<Event>& event);
        static void OnMouseRelease(std::unique_ptr<Event>& event);

        static void SetMouseCursor(const Math::Vec2f& newMousePos);
        static void CheckForHovering(const Math::Vec2f& newMousePos);
        static bool UpdateSelectedWindow(const Math::Vec2f& newMousePos);
    };
}
