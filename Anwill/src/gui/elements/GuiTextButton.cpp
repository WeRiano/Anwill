#include "GuiTextButton.h"
#include "utils/Profiler.h"

namespace Anwill {

    GuiTextButton::GuiTextButton(const std::shared_ptr<GuiStyling::Container>& containerStyle,
                                 const std::string& text, const std::function<void()>& callback,
                                 const std::shared_ptr<GuiStyling::TextButton>& style)
        : GuiElement(containerStyle),
          GuiText(containerStyle, text, AW_GUI_MAKE_STYLE(style, GuiStyling::TextButton)),
          GuiButton(containerStyle, {GuiText::GetWidth() + GuiStyling::TextButton::textPadding * 2.0f,
                                     containerStyle->elementHeight}, callback,
                    AW_GUI_CAST_STYLE(GuiText::m_Style, GuiStyling::TextButton)),
          m_Style(AW_GUI_CAST_STYLE(GuiText::m_Style, GuiStyling::TextButton))
    {}

    void GuiTextButton::Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                               const Timestamp& delta)
    {
        AW_PROFILE_FUNC();
        GuiButton::Render(assignedPos, assignedMaxSize, delta);

        Math::Vec2f padding = Math::Vec2f(GuiStyling::TextButton::textPadding, 0.0f);
        GuiText::Render(assignedPos + padding,
                      assignedMaxSize - padding,
                      delta);
    }

    bool GuiTextButton::IsHovering(const Math::Vec2f& mousePos) const
    {
        return GuiButton::IsHovering(mousePos);
    }

    float GuiTextButton::GetWidth() const
    {
        return (m_TextPos.X + GuiText::GetWidth() > GuiButton::GetWidth()) ? GuiText::GetWidth() : GuiButton::GetWidth();
    }

    unsigned int GuiTextButton::GetGridDepth() const
    {
        return GuiText::GetGridDepth();
    }

    void GuiTextButton::SetText(const std::string& text) {
        GuiText::SetText(text);
        GuiButton::SetWidth(GuiText::GetWidth() + GuiStyling::TextButton::textPadding * 2.0f);
    }
}