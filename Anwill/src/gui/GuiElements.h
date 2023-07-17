 #pragma once

#include <vector>
#include <functional>
#include <iostream>

#include "gui/GuiStyling.h"
#include "gfx/Mesh.h"
#include "gfx/Font.h"
#include "gfx/Renderer2D.h"
#include "math/Mat4f.h"
#include "utils/Utils.h"

namespace Anwill {

    class GuiIcon {
    public:
        static void RenderRightArrow(const Math::Vec2f& assignedPos,
                                     const Math::Vec2f& assignedSize,
                                     const Math::Vec2f& assignedMaxSize,
                                     const Math::Vec3f& color);
        static void RenderDownArrow(const Math::Vec2f& assignedPos,
                                    const Math::Vec2f& assignedSize,
                                    const Math::Vec2f& assignedMaxSize,
                                    const Math::Vec3f& color);
        static void RenderCross(const Math::Vec2f& assignedPos,
                                const Math::Vec2f& assignedSize,
                                const Math::Vec2f& assignedMaxSize,
                                const Math::Vec3f& color);
        static void RenderCheckmark(const Math::Vec2f& assignedPos,
                                    const Math::Vec2f& assignedSize,
                                    const Math::Vec2f& assignedMaxSize,
                                    const Math::Vec3f& color);
        static void RenderRectangle(const Math::Vec2f& assignedPos,
                                    const Math::Vec2f& assignedSize,
                                    const Math::Vec2f& assignedMaxSize,
                                    const Math::Vec3f& color);
    };

    class GuiTooltip {
    public:
        GuiTooltip(const std::string& tooltipText, unsigned int tooltipTextSize);

        void Render(const Math::Vec2f& mousePos, const Math::Vec2f& gameWindowSize);
    private:
        std::string m_TooltipText;
        float m_TooltipTextScale;
    };

    class GuiElement {
    public:
        GuiElement();

        virtual void Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize) = 0;
        /**
         * @brief Check if the mouse cursor is hovering the element
         */
        virtual bool IsHovering(const Math::Vec2f& mousePos) const = 0;
        virtual float GetWidth() const = 0;
        virtual unsigned int GetGridDepth() const = 0;

        virtual void StartHovering();
        virtual void OnHover(const Math::Vec2f& mousePos);
        virtual void StopHovering();

        virtual void StartPressing();
        virtual void OnPress(const Math::Vec2f& mousePos);
        virtual void Release(); // Stop Pressing

        virtual void OnHoverRender(const Math::Vec2f& mousePos, const Math::Vec2f& gameWindowSize);
        void EmplaceTooltip(const std::string& tooltipText, unsigned int tooltipTextSize);

    protected:
        bool m_IsHovered, m_IsPressed;

    private:
        std::unique_ptr<GuiTooltip> m_Tooltip;
    };

    class GuiText : public GuiElement {
    public:
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
        float m_TextScale, m_TextWidth; // Calculate TextScale each iteration? Cheap.
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
        GuiTextButton(const std::string& text,
                      unsigned int textSize, const std::function<void()>& callback);

        void Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize) override;
        void SetText(const std::string& text);

    protected:
        GuiText m_Text;
    };

    class GuiCheckbox : public GuiButton {
    public:
        GuiCheckbox(bool startAsChecked,
                    const std::function<void(bool)>& callback);

        void Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize) override;

    private:
        bool m_Checked;
    };

    class GuiSlider : public GuiButton {
    public:
        GuiSlider();

        void Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize) override;

        virtual void OnPress(const Math::Vec2f& mousePos) override;

    protected:
        GuiText m_ValueText;
        float m_LastCursorXPos;
    };

    class GuiFloatSlider : public GuiSlider {
    public:
        GuiFloatSlider(float min, float max, float* sliderValue);

        void OnPress(const Math::Vec2f& mousePos) override;

    private:
        float m_Min, m_Max;
        float* m_ClientValuePointer;
    };

    class GuiIntSlider : public GuiSlider {
    public:
        GuiIntSlider(int min, int max, int* sliderValue);

        void OnPress(const Math::Vec2f& mousePos) override;

    private:
        int m_Min, m_Max;
        int* m_ClientValuePointer;
    };

    class GuiContainer {
    public:
        GuiContainer();

        virtual std::shared_ptr<GuiElement> GetHoverElement(Math::Vec2f& hoverElementPos,
                                                            const Math::Vec2f& mousePos) const;
        void Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                    const Math::Vec2f& firstPos);
        bool IsHidingElements() const;

        template <class E, typename... Args>
        std::shared_ptr<E> AddElement(bool onNewRow, bool forceNextToNewRow, Args&&... args) {
            if(m_Elements.empty() || (onNewRow || m_NewRowChecks.back().second)) {
                m_GridDepth++;
            }
            m_Elements.emplace_back(std::make_shared<E>(std::forward<Args>(args)...));
            m_NewRowChecks.emplace_back(onNewRow, forceNextToNewRow);
            return std::dynamic_pointer_cast<E>(m_Elements.back());
        }

    protected:
        unsigned int m_GridDepth;
        std::vector<std::shared_ptr<GuiElement>> m_Elements;
        std::vector<Math::Vec2f> m_ElementPosCache;
        // first bool describes if the element wants to be on this row,
        // second bool describes if it wants the next element to be on a new row
        std::vector<std::pair<bool, bool>> m_NewRowChecks;
        volatile bool m_HideElements;
    };

    class GuiDropdown : public GuiTextButton, public GuiContainer {
        /*
         * Inherited behavior from GuiTextButton:
         * - Dropdown toggle is a button, but with maximum allowed width and an arrow icon
         */
    public:
        GuiDropdown(const std::string& text, unsigned int textSize);

        void Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize) override;
        bool IsHovering(const Math::Vec2f& mousePos) const override;
        float GetWidth() const override;
        unsigned int GetGridDepth() const override;
    };

    typedef unsigned int GuiWindowID;

    class GuiWindow : public GuiContainer {
    public:
        GuiWindow(const std::string& title, GuiWindowID id,
                  const Math::Vec2f& position, const Math::Vec2f& size);

        std::shared_ptr<GuiElement> GetHoverElement(Math::Vec2f& hoverElementPos,
                                                    const Math::Vec2f& mousePos) const override;
        void Render(bool selected);
        bool IsHoveringHeader(const Math::Vec2f& mousePos);
        bool IsHoveringResize(const Math::Vec2f& mousePos);
        bool IsHoveringWindow(const Math::Vec2f& mousePos);
        bool IsHoveringMinimize(const Math::Vec2f& mousePos);
        void Move(const Math::Vec2f& delta, const Math::Vec2f& minPos, const Math::Vec2f& maxPos);
        void Resize(const Math::Vec2f& delta, const Math::Vec2f& minSize, const Math::Vec2f& maxSize);
        Math::Vec2f GetPos() const;
        GuiWindowID GetID() const;

    private:
        Math::Vec2f m_Pos, m_Size, m_LastShowSize;
        GuiWindowID m_ID;
        GuiText m_Title;
        std::shared_ptr<GuiButton> m_MinimizeButton;
    };
}