#include "GuiDropdown.h"
#include "gui/elements/GuiIcon.h"
#include "utils/Profiler.h"

namespace Anwill {

    GuiDropdown::GuiDropdown(const std::shared_ptr<GuiStyling::Container>& containerStyle, const std::string& text,
                             unsigned int textSize, const std::shared_ptr<GuiStyling::Dropdown>& style)
        : GuiElement(containerStyle),
          GuiContainer(style == nullptr ? std::make_shared<GuiStyling::Dropdown>() : style,
                       false, 1),
          m_Style(std::static_pointer_cast<GuiStyling::Dropdown>(GuiContainer::m_Style)),
          m_Text(containerStyle, text, textSize, style),
          m_Button(containerStyle, {0.0f, containerStyle->elementHeight}, [this](){
              ToggleElementsVisibility();
          }, style)
    {
    }

    std::shared_ptr<GuiElement> GuiDropdown::GetHoverElement(Math::Vec2f& hoverElementPos,
                                                             const Math::Vec2f& mousePos) const
    {
        return GuiContainer::GetHoverElement(hoverElementPos, mousePos - m_Style->GetFirstElementPos());
    }

    void GuiDropdown::Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                             const Timestamp& delta)
    {
        AW_PROFILE_FUNC();
        // Set button size to the maximum allowed width
        m_Button.SetWidth(assignedMaxSize.X);
        m_Button.Render(assignedPos, assignedMaxSize, delta);

        // Render arrow icon
        if(IsShowingElements()) {
            GuiIcon::RenderDownArrow(assignedPos,
                                     m_Style->GetIconSize() * 0.5f,
                                     m_Style->iconColor);
        } else {
            GuiIcon::RenderRightArrow(assignedPos,
                                      m_Style->GetIconSize() * 0.5f,
                                      m_Style->iconColor);
        }

        // Render text slightly to the right compared to a regular text button
        m_Text.Render(assignedPos + Math::Vec2f(m_Style->GetIconSize().X, 0.0f),
                      assignedMaxSize - Math::Vec2f(m_Style->GetIconSize().X, 0.0f), delta);

        GuiContainer::Render(assignedPos + Math::Vec2f(m_Style->elementIndent, -m_Style->GetRowHeight()),
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
        return IsShowingElements() ? GuiContainer::GetGridDepth() : 1;
    }

    void GuiDropdown::OnPress(const Math::Vec2f& mousePos)
    {
        m_Button.OnPress(mousePos);
    }
}