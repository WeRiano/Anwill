#pragma once

#include <vector>
#include <functional>

#include "gfx/Mesh.h"
#include "gfx/Font.h"
#include "gfx/Renderer2D.h"
#include "math/Mat4f.h"

namespace Anwill {

    struct GuiMetrics {
        static constexpr float WindowElementHeight = 30.0f;
        static constexpr float TextBaselineOffset = WindowElementHeight * 0.68f;
        static constexpr float WindowBorderSize = 8.0f;
        static constexpr float WindowHeaderSize = WindowBorderSize * 2.5f;
        static constexpr float WindowElementMargin = 6.0f;
        static constexpr float WindowCutoffMargin = 3.0f;
        static constexpr float ButtonTextMargin = 5.0f; // X distance from button edge to text
    };

    class GuiElement {
    public:
        static Mesh s_RectMesh;
        static std::unique_ptr<Font> s_Font;

        GuiElement(const Math::Vec2f& position);

        virtual void Render(const Math::Vec2f& parentPos, const Math::Vec2f& parentSize) = 0;
        virtual bool IsHovering(const Math::Vec2f& mousePos) = 0;
        virtual void OnHover() = 0;
        virtual void OnPress() = 0;
        virtual void OnRelease() = 0;

        void Move(const Math::Vec2f& delta);
        void StopHovering();
        void StopPressing();
        Math::Vec2f GetPos() const;
        Math::Vec2f GetSize() const;

    protected:
        // This transform holds the position relative the top left corner of its window host
        Math::Vec2f m_Pos, m_Size;
        bool m_IsHovered, m_IsPressed;
    };

    class GuiText : public GuiElement {
    public:
        static std::shared_ptr<Shader> s_Shader;

        GuiText(const std::string& text, unsigned int textSize, const Math::Vec2f& position);

        void Render(const Math::Vec2f& parentPos, const Math::Vec2f& parentSize) override;
        bool IsHovering(const Math::Vec2f& mousePos) override;
        void OnHover() override;
        void OnPress() override;
        void OnRelease() override;

    private:
        std::string m_Text;
        float m_TextScale;
        float m_TextWidth;
    };

    class GuiButton : public GuiElement {
    public:
        static std::shared_ptr<Shader> s_Shader;

        GuiButton(const std::string& text, unsigned int textSize, const std::function<void()>& callback,
                  const Math::Vec2f& position);

        void Render(const Math::Vec2f& parentPos, const Math::Vec2f& parentSize) override;
        bool IsHovering(const Math::Vec2f& mousePos) override;
        void OnHover() override;
        void OnPress() override;
        void OnRelease() override;

    private:
        GuiText m_ButtonText;
        std::function<void()> m_Callback;
    };

    class GuiDropdown : public GuiElement {
        std::vector<std::shared_ptr<GuiElement>> m_Elements;

        void Render(const Math::Vec2f& parentPos, const Math::Vec2f& parentSize) override;
    };

    typedef unsigned int GuiWindowID;

    class GuiWindow {
    public:
        static std::shared_ptr<Shader> s_WindowShader;

        GuiWindow(const std::string& title, GuiWindowID id, const Math::Vec2f& position, const Math::Vec2f& size);

        void Render(bool selected);
        bool IsHoveringHeader(const Math::Vec2f& mousePos);
        bool IsHoveringResize(const Math::Vec2f& mousePos);
        bool IsHoveringWindow(const Math::Vec2f& mousePos);
        std::shared_ptr<GuiElement> GetHoverElement(const Math::Vec2f& mousePos);
        void Move(const Math::Vec2f& delta, const Math::Vec2f& minPos, const Math::Vec2f& maxPos);
        void Resize(const Math::Vec2f& delta, const Math::Vec2f& minSize, const Math::Vec2f& maxSize);
        Math::Vec2f GetPos();
        GuiWindowID GetID();

        template <class E, typename... Args>
        void AddHorizontalElement(Args&&... args) {
            // If this is the first element in the window it is technically always a new vertical element
            // (So we add it as such to correctly calculate correct values for nextHorizontalElementStart
            // and nextVerticalElementStart)
            if(m_Elements.empty()) {
                return AddVerticalElement<E>(std::forward<Args>(args)...);
            }

            m_Elements.emplace_back(std::make_shared<E>(std::forward<Args>(args)..., m_NextHorizontalElementPos));
            auto newElement = m_Elements[m_Elements.size() - 1];

            // New Vertical spot remains the same. New horizontal spot is 1 shift to the right.
            m_NextHorizontalElementPos = {
                    m_NextHorizontalElementPos.GetX() + newElement->GetSize().GetX() + GuiMetrics::WindowElementMargin,
                    m_NextHorizontalElementPos.GetY()
            };
        }

        template <class E, typename... Args>
        void AddVerticalElement(Args&&... args) {
            m_Elements.emplace_back(std::make_shared<E>(std::forward<Args>(args)..., m_NextVerticalElementPos));
            auto newElement = m_Elements[m_Elements.size() - 1];

            if(m_Elements.size() == 1) {
                // If this is the first element in the window we have to push it 1 to the right
                m_NextHorizontalElementPos = {
                        m_NextHorizontalElementPos.GetX() + newElement->GetSize().GetX() + GuiMetrics::WindowElementMargin,
                        m_NextHorizontalElementPos.GetY()
                };
            } else {
                // Otherwise its to the right of the new element
                m_NextHorizontalElementPos = {
                        m_NextVerticalElementPos.GetX() + newElement->GetSize().GetX() + GuiMetrics::WindowElementMargin,
                        m_NextVerticalElementPos.GetY()
                };
            }
            // Next vertical element is just 1 spot down
            m_NextVerticalElementPos = {
                    m_NextVerticalElementPos.GetX(),
                    m_NextVerticalElementPos.GetY() -(newElement->GetSize().GetY() + GuiMetrics::WindowElementMargin)
            };
        }

    private:
        Math::Vec2f m_Pos, m_Size;
        GuiWindowID m_ID;
        GuiText m_Title;
        std::vector<std::shared_ptr<GuiElement>> m_Elements;
        Math::Vec2f m_NextHorizontalElementPos, m_NextVerticalElementPos;
    };

}