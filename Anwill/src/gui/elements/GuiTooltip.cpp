#include "GuiTooltip.h"
#include "gui/styles/GuiStyling.h"
#include "utils/Profiler.h"
#include "gfx/Renderer2D.h"

namespace Anwill {

    GuiTooltip::GuiTooltip(const std::string& tooltipText, unsigned int tooltipTextSize)
        : m_TooltipText(tooltipText),
          m_TooltipTextScale(GuiStyling::Text::font->GetScaleValue(tooltipTextSize))
    {}

    void GuiTooltip::Render(const Math::Vec2f& mousePos, const Math::Vec2f& gameWindowSize)
    {
        AW_PROFILE_FUNC();
        // First render the tooltip background window
        Math::Vec2f backgroundTopLeftCorner = mousePos + Math::Vec2f(GuiStyling::Tooltip::offset, 0.0f);
        Math::Vec2f textStartPos = backgroundTopLeftCorner +
                                   Math::Vec2f(GuiStyling::Tooltip::windowMargin.X, -GuiStyling::Tooltip::windowMargin.Y) +
                                   Math::Vec2f(0.0f, -GuiStyling::Text::font->GetFontHeight()) * m_TooltipTextScale;

        Math::Vec2f textSize = GuiStyling::Text::font->GetTextSize(m_TooltipText) * m_TooltipTextScale;
        Math::Vec2f backgroundSize = textSize + GuiStyling::Tooltip::windowMargin * 2.0f;

        // Move to fit inside game window
        bool isOutsideX = backgroundTopLeftCorner.X + backgroundSize.X > gameWindowSize.X;
        bool isOutsideY = backgroundTopLeftCorner.Y - backgroundSize.Y < 0.0f;
        Math::Vec2f correctedOffset;
        if(isOutsideX and isOutsideY) {
            float correctedOffsetX = gameWindowSize.X -
                                     (backgroundTopLeftCorner.X + backgroundSize.X);
            correctedOffset = {correctedOffsetX, GuiStyling::Tooltip::offset + backgroundSize.Y};
        } else if (isOutsideX) {
            float correctedOffsetX = gameWindowSize.X -
                                     (backgroundTopLeftCorner.X + backgroundSize.X);
            correctedOffset = {correctedOffsetX, -GuiStyling::Tooltip::offset};
        } else if (isOutsideY) {
            float correctedOffsetY = -(backgroundTopLeftCorner.Y - backgroundSize.Y);
            correctedOffset = {0.0f, correctedOffsetY};
        }

        Math::Mat4f transform = Math::Mat4f::Scale({}, backgroundSize);
        transform = Math::Mat4f::Translate(transform, backgroundTopLeftCorner + correctedOffset +
                                                      Math::Vec2f(backgroundSize.X, -backgroundSize.Y) * 0.5f);
        Renderer2D::SubmitMesh(GuiStyling::Tooltip::shader, GuiStyling::rectMesh, transform);

        // Then render the tooltip text
        transform = Math::Mat4f::Scale({}, {m_TooltipTextScale, m_TooltipTextScale, 0.0f});
        transform = Math::Mat4f::Translate(transform, textStartPos + correctedOffset);
        Renderer2D::SubmitText(GuiStyling::Text::shader, *GuiStyling::Text::font, m_TooltipText,
                               transform);
    }
}