#include "GuiStyling.h"
#include "gfx/ShaderMacros.h"

namespace Anwill {

    float GuiStyling::windowHeaderSize, GuiStyling::windowElementIndent,
    GuiStyling::windowElementVerticalMargin, GuiStyling::windowElementHorizontalMargin,
    GuiStyling::windowCutoffPadding, GuiStyling::textBaselineOffset, GuiStyling::buttonTextPadding,
    GuiStyling::checkboxIconMargin, GuiStyling::windowElementHeight, GuiStyling::windowBorderSize,
    GuiStyling::tooltipBorderSize, GuiStyling::tooltipOffset;
    unsigned int GuiStyling::fontSize;
    Math::Vec2f GuiStyling::iconSize, GuiStyling::windowTitlePos,
    GuiStyling::tooltipWindowMargin, GuiStyling::sliderMarkerSize;
    std::unique_ptr<Font> GuiStyling::font;
    std::shared_ptr<Shader> GuiStyling::textShader, GuiStyling::textButtonShader, GuiStyling::primitiveShader,
    GuiStyling::windowShader, GuiStyling::tooltipShader;
    Mesh GuiStyling::checkmarkMesh, GuiStyling::triangleMesh, GuiStyling::rectMesh;

    void GuiStyling::Init()
    {
        // --- Text ---
        fontSize = 13;
        textBaselineOffset = -(float) fontSize * 0.45f;
        buttonTextPadding = 5.0f; // X distance from button edge to text

        // --- Checkbox ---
        checkboxIconMargin = 2.0f;

        // --- Window ---
        windowElementIndent = 5.0f;
        windowElementHeight = 30.0f;
        windowElementVerticalMargin = 3.0f;
        windowElementHorizontalMargin = 6.0f;
        windowCutoffPadding = 4.0f;
        windowBorderSize = 8.0f;
        windowHeaderSize = windowElementHeight;
        iconSize = { windowElementHeight, windowElementHeight };
        windowTitlePos = { windowBorderSize + iconSize.GetX(),
                           windowElementHeight / 2.0f - windowHeaderSize / 2.0f };
        ShaderMacros::SetMacro("AW_GUI_WINDOW_BORDER_SIZE", windowBorderSize);
        ShaderMacros::SetMacro("AW_GUI_WINDOW_HEADER_SIZE", windowHeaderSize);

        // --- Slider ---
        sliderMarkerSize = {13.0f, windowElementHeight - 1.5f * 2.0f};

        // --- Tooltip ---
        tooltipOffset = 15.0f;
        tooltipWindowMargin = {10.0f, 10.0f};
        tooltipBorderSize = 2.0f;
        ShaderMacros::SetMacro("AW_GUI_TOOLTIP_BORDER_SIZE", tooltipBorderSize);

        // Custom objects
            // Mesh
        rectMesh = Mesh::CreateRectMesh(1.0f, 1.0f);
        triangleMesh = Mesh::CreateTriangleMesh({0.0f, -0.5f}, {0.5f, 0.5f}, {-0.5f, 0.5f});
        checkmarkMesh = Mesh::CreateCheckmarkMesh(1.0f, 1.0f);

            // Shaders
        primitiveShader = Shader::Create("Anwill/res/shaders/OpenGL/GuiPrimitiveColor.glsl");
        GuiStyling::primitiveShader->Bind();
        GuiStyling::primitiveShader->SetUniformVec3f({1.0f, 1.0f, 1.0f}, "u_Color");
        GuiStyling::primitiveShader->Unbind();
        textShader = Shader::Create("Anwill/res/shaders/OpenGL/GuiText.glsl");
        textButtonShader = Shader::Create("Anwill/res/shaders/OpenGL/GuiTextButton.glsl");
        windowShader = Shader::Create("Anwill/res/shaders/OpenGL/GuiWindow.glsl");
        tooltipShader = Shader::Create("Anwill/res/shaders/OpenGL/GuiTooltip.glsl");

            // Fonts
        font = std::make_unique<Font>("Sandbox/assets/fonts/arial.ttf");
        font->SetNewlineSpace(1.4f);
    }
}