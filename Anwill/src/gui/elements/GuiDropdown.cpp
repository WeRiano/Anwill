#include "GuiDropdown.h"
#include "gui/elements/GuiIcon.h"
#include "utils/Profiler.h"

namespace Anwill {

    GuiDropdown::GuiDropdown(const std::shared_ptr<GuiStyling::Container>& containerStyle, const std::string& text,
                             const std::shared_ptr<GuiStyling::Dropdown>& style)
        : GuiElement(containerStyle),
          GuiTextButton(containerStyle, text, [this](){
              ToggleElementsVisibility();
          }, AW_GUI_ENSURE_STYLE(style, GuiStyling::Dropdown)),
          GuiContainer(AW_GUI_CAST_STYLE(GuiTextButton::m_Style, GuiStyling::Dropdown),false, 1),
          m_Style(AW_GUI_CAST_STYLE(GuiTextButton::m_Style, GuiStyling::Dropdown))
    {
        m_TextPos += Math::Vec2f(m_Style->GetIconSize().X, 0.0f);
    }

    void GuiDropdown::Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                             const Timestamp& delta)
    {
        AW_PROFILE_FUNC();
        //RenderDebugGrid(assignedPos, assignedMaxSize);
        // Set button size to the maximum allowed width
        GuiButton::SetWidth(assignedMaxSize.X);
        // Render text button
        GuiTextButton::Render(assignedPos, assignedMaxSize, delta);
        // Render the additional icon
        if(IsShowingElements()) {
            GuiIcon::RenderDownArrow(assignedPos,
                                     m_Style->GetIconSize() * 0.5f,
                                     m_Style->iconColor);
        } else {
            GuiIcon::RenderRightArrow(assignedPos,
                                      m_Style->GetIconSize() * 0.5f,
                                      m_Style->iconColor);
        }
        // Render dropdown elements
        GuiContainer::Render(assignedPos + m_Style->GetFirstElementPos(),
                             assignedMaxSize - m_Style->GetFirstElementPos().NegateY(), delta);
    }

    unsigned int GuiDropdown::GetGridDepth() const
    {
        return IsShowingElements() ? GuiContainer::GetGridDepth() : 1;
    }
}