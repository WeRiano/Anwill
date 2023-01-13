 #pragma once

#include <vector>
#include <functional>
#include <iostream>

#include "gfx/Mesh.h"
#include "gfx/Font.h"
#include "gfx/Renderer2D.h"
#include "math/Mat4f.h"

namespace Anwill {

    struct GuiMetrics {
        static const unsigned int FontSize = 13;

            // Window element stuff
        static constexpr float WindowElementIndent = 5.0f;
        static constexpr float WindowElementHeight = 30.0f;
        static constexpr float WindowElementVerticalMargin = 3.0f;
        static constexpr float WindowElementHorizontalMargin = 6.0f;
        static constexpr float WindowCutoffMargin = 4.0f;
        // We render text in the middle of the assigned space, but the baseline should not be in the middle, it should be slightly below
        static constexpr float TextBaselineOffset = -(float) FontSize * 0.45f;
        static constexpr float ButtonTextMargin = 5.0f; // X distance from button edge to text
        static constexpr float CheckboxElementMargin = 2.0f;

            // Window stuff
        static constexpr float WindowBorderSize = 8.0f;
        static constexpr float WindowHeaderSize = WindowElementHeight;

        static Math::Vec2f windowSize;

        static inline Math::Vec2f GetNextElementPos(const Math::Vec2f& curPos,
                                                    float curElementWidth,
                                                    unsigned int curElementGridDepth,
                                                    float originXPos, bool onNewRow) {
            if(onNewRow) {
                return {
                        originXPos,
                        curPos.GetY() - ((float) curElementGridDepth * (GuiMetrics::WindowElementHeight + GuiMetrics::WindowElementVerticalMargin))
                };
            } else {
                return {
                        curPos.GetX() + curElementWidth + GuiMetrics::WindowElementHorizontalMargin,
                        curPos.GetY()
                };
            }
        }

        static inline Math::Vec2f GetCutoffPos(const Math::Vec2f& assignedPos,
                                               const Math::Vec2f& assignedMaxSize) {
            return {assignedPos.GetX() + assignedMaxSize.GetX(), assignedPos.GetY() - assignedMaxSize.GetY()};
        }

        static inline Math::Vec2f GetNewMaxSize(const Math::Vec2f& posDelta,
                                                const Math::Vec2f& oldMaxSize) {
            return {oldMaxSize.GetX() - posDelta.GetX(), oldMaxSize.GetY() + posDelta.GetY()};
        }
    };

    class GuiIcon {
    public:
        static void RenderRightArrow(const Math::Vec2f& assignedPos,
                                     const Math::Vec2f& assignedSize,
                                     const Math::Vec2f& assignedMaxSize);
        static void RenderDownArrow(const Math::Vec2f& assignedPos,
                                     const Math::Vec2f& assignedSize,
                                     const Math::Vec2f& assignedMaxSize);
        static void RenderCross(const Math::Vec2f& assignedPos,
                                     const Math::Vec2f& assignedSize,
                                     const Math::Vec2f& assignedMaxSize);
        static void RenderCheckmark(const Math::Vec2f& assignedPos,
                                   const Math::Vec2f& assignedSize,
                                   const Math::Vec2f& assignedMaxSize);
    };

    class GuiElement {
    public:
        static Mesh s_RectMesh;
        static Mesh s_TriangleMesh;
        static std::shared_ptr<Shader> s_PrimitiveShader;
        static std::unique_ptr<Font> s_Font;

        GuiElement();

        virtual void Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize) = 0;
        /**
         * @brief Check if the mouse cursor is hovering the element
         */
        virtual bool IsHovering(const Math::Vec2f& mousePos) const = 0;
        virtual float GetWidth() const = 0;
        virtual unsigned int GetGridDepth() const = 0;

        virtual void StartHovering();
        virtual void StopHovering();
        virtual void StartPressing();
        virtual void StopPressing();
        virtual void Release();

        /**
         * @brief Check if element is currently in the state of being hovered by something
         */
        bool IsHovered() const;

    protected:
        bool m_IsHovered, m_IsPressed;
    };

    class GuiText : public GuiElement {
    public:
        static std::shared_ptr<Shader> s_Shader;

        GuiText(const std::string& text, unsigned int textSize);

        void Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize) override;
        bool IsHovering(const Math::Vec2f& mousePos) const override;
        float GetWidth() const override;
        unsigned int GetGridDepth() const override;

        virtual void SetText(const std::string& text);

    protected:
        Math::Vec2f m_TextPos;

    private:
        std::string m_Text;
        float m_TextScale; // Scaling factor to render in given textSize
        float m_TextWidth; // Horizontal space occupied by the text
    };

    class GuiButton : public GuiElement {
    public:
        GuiButton(const Math::Vec2f& size,
                  const std::function<void()>& callback);

        void Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize) override;
        bool IsHovering(const Math::Vec2f& mousePos) const override;
        float GetWidth() const override;
        unsigned int GetGridDepth() const override;
        void Release() override;

        void SetCallback(const std::function<void()>& callback);

    protected:
        Math::Vec2f m_ButtonSize;
        std::function<void()> m_Callback;
    };

    class GuiTextButton : public GuiButton {
        /*
         * Inherited behavior from GuiText:
         *  - Has text
         *  - Renders based on width of text
         */
    public:
        static std::shared_ptr<Shader> s_Shader;

        GuiTextButton(const std::string& text,
                      unsigned int textSize, const std::function<void()>& callback);

        void Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize) override;
        void SetText(const std::string& text);

    protected:
        GuiText m_Text;
    };

    class GuiCheckbox : public GuiButton {
    public:
        static Mesh s_CheckmarkMesh;

        GuiCheckbox(bool startAsChecked,
                    const std::function<void(bool)>& callback);

        void Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize) override;
    private:
        bool m_Checked;
    };

    class GuiSlider : public GuiElement {

    };
}