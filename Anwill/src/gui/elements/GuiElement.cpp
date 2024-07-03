#include "GuiElement.h"

namespace Anwill {

    GuiElement::GuiElement(const std::shared_ptr<GuiStyling::Container>& containerStyle)
        : m_IsHovered(false), m_IsPressed(false), m_IsSelected(false), m_ContainerStyle(containerStyle)
    {}

    void GuiElement::StartHovering()
    {
        m_IsHovered = true;
    }

    void GuiElement::StopHovering()
    {
        m_IsHovered = false;
    }

    void GuiElement::OnHover(const Math::Vec2f& mousePos)
    {}

    void GuiElement::StartPressing()
    {
        m_IsPressed = true;
    }

    void GuiElement::OnPress(const Math::Vec2f& mousePos)
    {}

    void GuiElement::Release()
    {
        m_IsPressed = false;
    }

    void GuiElement::Select()
    {
        m_IsSelected = true;
    }

    void GuiElement::Deselect()
    {
        m_IsSelected = false;
    }

    void GuiElement::OnKeyPress(const KeyCode& keyCode)
    {}

    void GuiElement::OnKeyRepeat(const KeyCode& keyCode)
    {}

    void GuiElement::OnKeyRelease(const KeyCode& keyCode)
    {}

    void GuiElement::OnKeyChar(unsigned char)
    {}

    void GuiElement::OnHoverRender(const Math::Vec2f& mousePos, const Math::Vec2f& gameWindowSize)
    {
        if (m_Tooltip != nullptr)
        {
            m_Tooltip->Render(mousePos, gameWindowSize);
        }
    }

    void GuiElement::EmplaceTooltip(const std::string& tooltipText)
    {
        m_Tooltip = std::make_unique<GuiTooltip>(tooltipText, GuiStyling::Text::fontSize);
    }
}
