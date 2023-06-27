#pragma once

#include "math/Vec2f.h"
#include "gfx/Mesh.h"
#include "gfx/Shader.h"
#include "gfx/Font.h"

namespace Anwill {

    /**
     * Colors, margins, padding, shaders, meshes, etc.
     */
    struct GuiStyling
    {
        // --- Icon ---
        static Math::Vec2f iconSize;
        static Mesh rectMesh, triangleMesh;
        static std::shared_ptr<Shader> primitiveShader;

        // --- Text ---
        static unsigned int fontSize;
        // We render text in the middle of the assigned space, but the baseline should not be in the middle,
        // it should be slightly below
        static float textBaselineOffset;
        static float buttonTextPadding; // X distance from button edge to text
        static std::unique_ptr<Font> font;
        static std::shared_ptr<Shader> textShader, textButtonShader;

        // --- Checkbox ---
        static float checkboxElementMargin;
        static Mesh checkmarkMesh;

        // --- Slider ---
        static Math::Vec2f sliderMarkerSize;

        // --- Tooltip ---
        // Offset from cursor to the top left corner of the tooltip window
        static Math::Vec2f tooltipOffset;
        // Padding between the edge of the tooltip window and the tooltip text
        static Math::Vec2f tooltipTextPadding;
        static std::shared_ptr<Shader> tooltipShader;

        // --- Dropdown ---


        // --- Window ---
        static float windowElementIndent;
        static float windowElementHeight;
        static float windowElementVerticalMargin, windowElementHorizontalMargin;
        static float windowCutoffPadding;
        static float windowBorderSize, windowHeaderSize;
        static Math::Vec2f windowTitlePos;
        static std::shared_ptr<Shader> windowShader;

        static void Init();
    };
}
