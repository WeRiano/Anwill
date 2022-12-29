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
        static constexpr float WindowCutoffMargin = 2.0f;
        // We render text in the middle of the assigned space, but the baseline should not be in the middle, it should be slightly below
        static constexpr float TextBaselineOffset = -(float) FontSize * 0.45;
        static constexpr float ButtonTextMargin = 5.0f; // X distance from button edge to text

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
    };

    class GuiElement {
    public:
        static Mesh s_RectMesh;
        static Mesh s_TriangleMesh;
        static std::shared_ptr<Shader> s_PrimitiveShader;
        static std::unique_ptr<Font> s_Font;

        GuiElement(bool onNewRow, bool forceNextToNewRow);

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
        bool OnNewRow() const;
        bool ForceNextToNewRow() const;

    protected:
        // Position is relative to assigned grid position.
        // Size is absolute and tells manager how much space you occupy.
        // If m_ForceNextToNewRow is true your width will not be used
        bool m_IsHovered, m_IsPressed;
        bool m_OnNewRow, m_ForceNextToNewRow;
    };

    class GuiText : public GuiElement {
    public:
        static std::shared_ptr<Shader> s_Shader;

        GuiText(bool onNewRow, const std::string& text, unsigned int textSize);

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

    class GuiIcon : public GuiElement {
    public:
        // static stuff

        enum class Type {
            Arrow = 1,
            CheckboxStandard,
            CheckboxCross,
            Exit
        };

        GuiIcon();

        virtual void OnStartHovering() = 0;
        virtual void OnStartPressing() = 0;
        virtual void OnStopHovering() = 0;
        virtual void OnStopPressing() = 0;
        virtual void OnRelease() = 0;

    protected:
        Math::Vec2f m_Pos, m_Size;
    };

    class GuiTextButton : public GuiText {
        /*
         * Inherited behavior from GuiText:
         *  - Has text
         *  - Renders based on width of text
         */
    public:
        static std::shared_ptr<Shader> s_Shader;

        GuiTextButton(bool onNewRow, const std::string& text,
                      unsigned int textSize, const std::function<void()>& callback);

        void Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize) override;
        bool IsHovering(const Math::Vec2f& mousePos) const override;
        float GetWidth() const override;
        unsigned int GetGridDepth() const override;
        void Release() override;
        void SetText(const std::string& text) override;

        void SetCallback(const std::function<void()>& callback);

    protected:
        Math::Vec2f m_ButtonSize;

    private:
        std::function<void()> m_Callback;
    };
}