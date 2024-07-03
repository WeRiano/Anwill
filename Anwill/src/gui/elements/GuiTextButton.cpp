#include "GuiTextButton.h"
#include "utils/Profiler.h"

namespace Anwill {

    GuiTextButton::GuiTextButton(const std::shared_ptr<GuiStyling::Container>& containerStyle,
                                 const std::string& text, unsigned int textSize, const std::function<void()>& callback,
                                 const std::shared_ptr<GuiStyling::TextButton>& style)
        : GuiElement(containerStyle),
          m_Style(style == nullptr ? std::make_shared<GuiStyling::TextButton>() : style),
          m_Text(containerStyle, text, textSize, style),
          m_Button(containerStyle, {m_Text.GetWidth() + GuiStyling::TextButton::textPadding * 2.0f,
                                    containerStyle->elementHeight}, callback, style)
    {}

    GuiTextButton::GuiTextButton(const std::shared_ptr<GuiStyling::Container>& containerStyle, const std::string& text,
                                 unsigned int textSize, unsigned int pixelWidth, const std::function<void()>& callback,
                                 const std::shared_ptr<GuiStyling::TextButton>& style)
        : GuiElement(containerStyle),
          m_Text(containerStyle, text, textSize, style),
          m_Style(style == nullptr ? std::make_shared<GuiStyling::TextButton>() : style),
          m_Button(containerStyle, {static_cast<float>(pixelWidth),containerStyle->elementHeight}, callback)
    {}

    void GuiTextButton::Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                               const Timestamp& delta)
    {
        AW_PROFILE_FUNC();
        m_Button.Render(assignedPos, assignedMaxSize, delta);

        Math::Vec2f padding = Math::Vec2f(GuiStyling::TextButton::textPadding, 0.0f);
        m_Text.Render(assignedPos + padding,
                      assignedMaxSize - padding,
                      delta);
    }

    void GuiTextButton::SetText(const std::string& text) {
        m_Text.Set(text);
        m_Button.SetWidth(m_Text.GetWidth() + GuiStyling::TextButton::textPadding * 2.0f);
    }

    bool GuiTextButton::IsHovering(const Math::Vec2f& mousePos) const
    {
        return m_Button.IsHovering(mousePos);
    }

    float GuiTextButton::GetWidth() const
    {
        return m_Button.GetWidth();
    }

    unsigned int GuiTextButton::GetGridDepth() const
    {
        return 1;
    }

    Math::Vec2f GuiTextButton::GetSize() const
    {
        return m_Button.GetSize();
    }

    void GuiTextButton::SetCallback(const std::function<void()>& callback)
    {
        m_Button.SetCallback(callback);
    }

    void GuiTextButton::Release()
    {
        m_Button.Release();
    }
}