#pragma once

#include "gui/GuiElements.h"

namespace Anwill {

    class GuiContainer {
    public:
        GuiContainer();

        virtual std::shared_ptr<GuiElement> GetHoverElement(const Math::Vec2f& mousePos) const = 0;
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

        virtual std::shared_ptr<GuiElement> GetHoverElementInternal(const Math::Vec2f& mousePos,
                                                                    const Math::Vec2f& posOffset) const;
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
        std::shared_ptr<GuiElement> GetHoverElement(const Math::Vec2f& mousePos) const override;

    private:
        static constexpr float s_IconWidthHeight = GuiMetrics::WindowElementHeight;
    };

    typedef unsigned int GuiWindowID;

    class GuiWindow : public GuiContainer {
    public:
        static std::shared_ptr<Shader> s_WindowShader;

        GuiWindow(const std::string& title, GuiWindowID id,
                  const Math::Vec2f& position, const Math::Vec2f& size);

        std::shared_ptr<GuiElement> GetHoverElement(const Math::Vec2f& mousePos) const override;

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
        static constexpr float s_IconWidthHeight = GuiMetrics::WindowHeaderSize;
        static const Math::Vec2f s_TitlePos;

        Math::Vec2f m_Pos, m_Size, m_LastShowSize;
        GuiWindowID m_ID;
        GuiText m_Title;
        std::shared_ptr<GuiButton> m_MinimizeButton;
    };

}