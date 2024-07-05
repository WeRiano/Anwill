#pragma once

#include "GuiContainer.h"

namespace Anwill {

    //typedef unsigned int GuiWindowID;

    class GuiWindow : public GuiContainer {
    public:
        std::shared_ptr<GuiStyling::Window> m_Style;

        GuiWindow(const std::string& title, GuiWindowID id,
                  const Math::Vec2f& position, const Math::Vec2f& size);

        std::shared_ptr<GuiElement> GetHoverElement(Math::Vec2f& hoverElementPos,
                                                    const Math::Vec2f& mousePos) const override;
        void Render(bool isSelected, const Timestamp& delta);
        bool IsHoveringHeader(const Math::Vec2f& mousePos);
        bool IsHoveringResize(const Math::Vec2f& mousePos);
        bool IsHoveringWindow(const Math::Vec2f& mousePos);
        bool IsHoveringMinimize(const Math::Vec2f& mousePos);
        void Move(const Math::Vec2f& delta, const Math::Vec2f& minPos, const Math::Vec2f& maxPos);
        void Resize(const Math::Vec2f& delta, const Math::Vec2f& minSize, const Math::Vec2f& maxSize);
        void ScrollUp();
        void ScrollDown();
        Math::Vec2f GetPos() const;
        GuiWindowID GetID() const;

    private:
        //GuiContainer m_Container;
        Math::Vec2f m_Pos, m_Size, m_LastShowSize;
        GuiWindowID m_ID;
        GuiText m_Title;
        std::shared_ptr<GuiButton> m_MinimizeButton;
    };
}
