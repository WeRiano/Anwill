#pragma once

#include <functional>

#include "math/Vec2f.h"
#include "gui/styles/GuiStyling.h"

namespace Anwill {

    /**
     * Static functions to draw different icons.
     */
    class GuiIcon {
    public:
        typedef std::function<void(const Math::Vec2f&, const Math::Vec2f&, const Math::Vec3f&)> RenderIconFunctionType;

        static void RenderRightArrow(const Math::Vec2f& assignedPos,
                                     const Math::Vec2f& assignedSize,
                                     const Math::Vec3f& color);
        static void RenderDownArrow(const Math::Vec2f& assignedPos,
                                    const Math::Vec2f& assignedSize,
                                    const Math::Vec3f& color);
        static void RenderCross(const Math::Vec2f& assignedPos,
                                const Math::Vec2f& assignedSize,
                                const Math::Vec3f& color);
        static void RenderCheckmark(const Math::Vec2f& assignedPos,
                                    const Math::Vec2f& assignedSize,
                                    const Math::Vec3f& color);
        static void RenderRectangle(const Math::Vec2f& assignedPos,
                                    const Math::Vec2f& assignedSize,
                                    const Math::Vec3f& color);
        static void RenderEllipse(const Math::Vec2f& assignedPos,
                                  const Math::Vec2f& assignedSize,
                                  const Math::Vec3f& color);

        static const std::array<RenderIconFunctionType,
            (size_t) GuiStyling::Checkbox::CheckmarkType::Size> renderIconFunctions;
    };
}
