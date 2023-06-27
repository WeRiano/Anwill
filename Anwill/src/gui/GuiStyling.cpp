#include "GuiStyling.h"

namespace Anwill {

    float GuiStyling::windowHeaderSize, GuiStyling::windowElementIndent,
    GuiStyling::windowElementVerticalMargin, GuiStyling::windowElementHorizontalMargin,
    GuiStyling::windowCutoffPadding, GuiStyling::textBaselineOffset, GuiStyling::buttonTextPadding,
    GuiStyling::checkboxElementMargin, GuiStyling::windowElementHeight, GuiStyling::windowBorderSize;
    unsigned int GuiStyling::fontSize;
    Math::Vec2f GuiStyling::iconSize, GuiStyling::windowTitlePos;
    std::unique_ptr<Font> GuiStyling::font;
    std::shared_ptr<Shader> GuiStyling::textShader, GuiStyling::textButtonShader, GuiStyling::primitiveShader,
    GuiStyling::windowShader, GuiStyling::tooltipShader;
    Mesh GuiStyling::checkmarkMesh, GuiStyling::triangleMesh, GuiStyling::rectMesh;
    Math::Vec2f GuiStyling::sliderMarkerSize, GuiStyling::tooltipOffset;

    void GuiStyling::Init()
    {
        // --- Icon ---
        iconSize = { windowElementHeight, windowElementHeight };
        rectMesh = Mesh::CreateRectMesh(1.0f, 1.0f);
        triangleMesh = Mesh::CreateTriangleMesh({0.0f, -0.5f}, {0.5f, 0.5f}, {-0.5f, 0.5f});
        primitiveShader = Shader::Create("Anwill/res/shaders/OpenGL/GuiPrimitiveColor.glsl");
        GuiStyling::primitiveShader->Bind();
        GuiStyling::primitiveShader->SetUniformVec3f({1.0f, 1.0f, 1.0f}, "u_Color");
        GuiStyling::primitiveShader->Unbind();

        // --- Generic element ---
        windowElementIndent = 5.0f;
        windowElementHeight = 30.0f;
        windowElementVerticalMargin = 3.0f;
        windowElementHorizontalMargin = 6.0f;
        windowCutoffPadding = 4.0f;

        // --- Text ---
        fontSize = 13;
        textBaselineOffset = -(float) fontSize * 0.45f;
        buttonTextPadding = 5.0f; // X distance from button edge to text
        font = std::make_unique<Font>("Sandbox/assets/fonts/arial.ttf");
        textShader = Shader::Create("Anwill/res/shaders/OpenGL/GuiText.glsl");
        textButtonShader = Shader::Create("Anwill/res/shaders/OpenGL/GuiTextButton.glsl");

        // --- Checkbox ---
        checkboxElementMargin = 2.0f;
        checkmarkMesh = Mesh::CreateCheckmarkMesh(1.0f, 1.0f);

        // --- Window ---
        windowElementIndent = 5.0f;
        windowElementHeight = 30.0f;
        windowElementVerticalMargin = 3.0f;
        windowElementHorizontalMargin = 6.0f;
        windowCutoffPadding = 4.0f;
        windowBorderSize = 8.0f;
        windowHeaderSize = windowElementHeight;
        windowTitlePos = {GuiStyling::windowBorderSize + iconSize.GetX(),
                    GuiStyling::windowElementHeight / 2.0f - GuiStyling::windowHeaderSize / 2.0f};
        windowShader = Shader::Create("Anwill/res/shaders/OpenGL/GuiWindow.glsl");

        // --- Slider ---
        sliderMarkerSize = {13.0f, GuiStyling::windowElementHeight - 1.5f * 2.0f};

        // --- Tooltip ---
        tooltipOffset = {10.0f, 10.0f};
        tooltipShader = Shader::Create("Anwill/res/shaders/OpenGL/GuiTooltip.glsl");

    }


}