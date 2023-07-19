#pragma once

#include "math/Vec2f.h"
#include "gfx/Mesh.h"
#include "gfx/Shader.h"
#include "gfx/Font.h"

namespace Anwill {

    /**
     * Colors, margins, padding, shaders, meshes, etc. Anything that alters the look of the GUI.
     *
     * static members represent global styling, while non-static members are
     */
    struct GuiStyling
    {
        // --- Global/Shared members (Icon and more) ---
        static Math::Vec2f iconSize;
        static Mesh rectMesh, triangleMesh;
        static std::shared_ptr<Shader> primitiveShader;

        struct Tooltip {
        public:
            // Offset from cursor to the top left corner of the tooltip window
            static float offset;
            static float borderSize;
            // Padding between the edge of the tooltip window and the tooltip text.
            static Math::Vec2f windowMargin;
            static std::shared_ptr<Shader> shader;
        };

        struct Text {
        public:
            static unsigned int fontSize;
            // We render text in the middle of the assigned space, but the baseline should not be in the middle,
            // it should be slightly below
            static float baselineOffset;
            static std::unique_ptr<Font> font;
            static std::shared_ptr<Shader> shader;
        };

        struct Button {
        public:
            enum class Shape : short {
                Rectangle = 0,
                Circle,

                Size
            };

            // Index with Button::Shape
            static std::array<std::shared_ptr<Shader>, (size_t) Shape::Size> shaders;

            Shape shape = Shape::Rectangle;
            Math::Vec3f color = {0.15f, 0.15f, 0.6f};
            Math::Vec3f hoverColor = {0.20f, 0.20f, 0.95f};
            Math::Vec3f pressColor = {0.28f, 0.28f, 1.00f};
        };

        struct TextButton {
        public:
            static float textPadding; // X distance from button edge to text
        };

        struct Checkbox {
        public:
            static float textMargin; // Distance between the checkbox button and the text
            static float iconMargin; // Distance between icon and the edge of the box
            static Mesh checkmarkMesh;
        };

        struct Slider {
        public:
            static Math::Vec2f markerSize;
        };

        struct Dropdown {
        public:
            static float elementIndent;
        };

        struct Window {
        public:
            static float elementIndent;
            static float elementHeight;
            static float elementVerticalMargin, elementHorizontalMargin;
            static float cutoffPadding;
            static float borderSize, headerSize;
            static Math::Vec2f titlePos;
            static std::shared_ptr<Shader> shader;
        };

        static void InitGlobalStyling();
    };
}
