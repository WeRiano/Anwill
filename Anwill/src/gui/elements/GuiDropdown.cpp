#include "GuiDropdown.h"
#include "gui/elements/GuiIcon.h"
#include "utils/Profiler.h"

namespace Anwill {

    GuiDropdown::GuiDropdown(const std::shared_ptr<GuiStyling::Container>& containerStyle, const std::string& text,
                             unsigned int textSize, const std::shared_ptr<GuiStyling::Dropdown>& style)
        : GuiElement(containerStyle),
          m_Style(style == nullptr ? std::make_shared<GuiStyling::Dropdown>() : style),
          m_Container(style, false, 1),
          m_Text(containerStyle, text, textSize, style),
          m_Button(containerStyle, {0.0f, containerStyle->elementHeight}, [this](){
              m_Container.ToggleElementsVisibility();
          }, style)
    {}

    std::shared_ptr<GuiElement> GuiDropdown::GetHoverElement(Math::Vec2f& hoverElementPos,
                                                             const Math::Vec2f& mousePos) const
    {
        return m_Container.GetHoverElement(hoverElementPos, mousePos - m_Style->GetFirstElementPos());
    }

    void GuiDropdown::Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                             const Timestamp& delta)
    {
        AW_PROFILE_FUNC();
        // Set button size to the maximum allowed width
        m_Button.SetWidth(assignedMaxSize.X);
        m_Button.Render(assignedPos, assignedMaxSize, delta);

        // Render arrow icon
        if(m_Container.IsShowingElements()) {
            GuiIcon::RenderRightArrow(assignedPos,
                                      m_Style->GetIconSize() * 0.5f,
                                      m_Style->iconColor);
        } else {
            GuiIcon::RenderDownArrow(assignedPos,
                                     m_Style->GetIconSize() * 0.5f,
                                     m_Style->iconColor);
        }

        // Render text slightly to the right compared to a regular text button
        m_Text.Render(assignedPos + Math::Vec2f(m_Style->GetIconSize().X, 0.0f),
                      assignedMaxSize - Math::Vec2f(m_Style->GetIconSize().X, 0.0f), delta);

        if(m_Container.IsShowingElements()) { return; }
        m_Container.Render(assignedPos + Math::Vec2f(m_Style->elementIndent, -m_Style->GetRowHeight()),
                           assignedMaxSize, delta);
    }

    bool GuiDropdown::IsHovering(const Math::Vec2f& mousePos) const
    {
        return m_Button.IsHovering(mousePos);
    }

    float GuiDropdown::GetWidth() const
    {
        return m_Button.GetWidth();
    }

    unsigned int GuiDropdown::GetGridDepth() const
    {
        return m_Container.IsShowingElements() ? 1 : m_Container.GetGridDepth();
    }
}