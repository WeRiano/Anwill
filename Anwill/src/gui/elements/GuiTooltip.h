#pragma once

#include <string>

#include "math/Vec2f.h"

namespace Anwill {

    class GuiTooltip {
    public:
        GuiTooltip(const std::string& tooltipText, unsigned int tooltipTextSize);

        void Render(const Math::Vec2f& mousePos, const Math::Vec2f& gameWindowSize);
    private:
        std::string m_TooltipText;
        float m_TooltipTextScale;
    };
}