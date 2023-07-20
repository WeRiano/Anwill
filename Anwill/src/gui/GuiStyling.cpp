#include "GuiStyling.h"
#include "gfx/ShaderMacros.h"

namespace Anwill {

    float GuiStyling::Window::headerSize, GuiStyling::Window::elementIndent,
    GuiStyling::Window::elementVerticalMargin, GuiStyling::Window::elementHorizontalMargin,
    GuiStyling::Window::cutoffPadding, GuiStyling::Text::baselineOffset, GuiStyling::TextButton::textPadding,
    GuiStyling::Checkbox::iconMargin, GuiStyling::Window::elementHeight, GuiStyling::Window::borderSize,
    GuiStyling::Tooltip::borderSize, GuiStyling::Tooltip::offset, GuiStyling::Checkbox::textMargin,
    GuiStyling::Dropdown::elementIndent;

    unsigned int GuiStyling::Text::fontSize;

    Math::Vec2f GuiStyling::iconSize, GuiStyling::Window::titlePos,
    GuiStyling::Tooltip::windowMargin, GuiStyling::Slider::markerSize;

    Math::Vec3f GuiStyling::iconColor;

    std::unique_ptr<Font> GuiStyling::Text::font;

    std::shared_ptr<Shader> GuiStyling::Text::shader, GuiStyling::primitiveShader, GuiStyling::circleShader,
    GuiStyling::Window::shader, GuiStyling::Tooltip::shader;

    Mesh GuiStyling::checkmarkMesh, GuiStyling::triangleMesh, GuiStyling::rectMesh;

    std::array<std::shared_ptr<Shader>, (size_t) GuiStyling::Button::Shape::Size> GuiStyling::Button::shaders;

    void GuiStyling::InitGlobalStyling()
    {
        // Icon
        iconColor = {1.0f, 1.0f, 1.0f};

        // --- Tooltip ---
        Tooltip::offset = 15.0f;
        Tooltip::windowMargin = {10.0f, 10.0f};
        Tooltip::borderSize = 2.0f;
        ShaderMacros::SetMacro("AW_GUI_TOOLTIP_BORDER_SIZE", Tooltip::borderSize);

        // --- Text ---
        Text::fontSize = 13;
        Text::baselineOffset = -(float) Text::fontSize * 0.45f;
        TextButton::textPadding = 5.0f; // X distance from button edge to text

        // --- Checkbox ---
        Checkbox::textMargin = 5.0f;
        Checkbox::iconMargin = 5.0f;

        // --- Dropdown --
        Dropdown::elementIndent = 10.0f;

        Window::elementIndent = 5.0f;
        Window::elementHeight = 30.0f;
        Window::elementVerticalMargin = 3.0f;
        Window::elementHorizontalMargin = 6.0f;
        Window::cutoffPadding = 4.0f;
        Window::borderSize = 8.0f;
        Window::headerSize = Window::elementHeight;
        iconSize = { Window::elementHeight, Window::elementHeight };
        Window::titlePos = { Window::borderSize + iconSize.GetX(),
                           Window::elementHeight / 2.0f - Window::headerSize / 2.0f };
        ShaderMacros::SetMacro("AW_GUI_WINDOW_BORDER_SIZE", Window::borderSize);
        ShaderMacros::SetMacro("AW_GUI_WINDOW_HEADER_SIZE", Window::headerSize);

        // --- Slider ---
        Slider::markerSize = {13.0f, Window::elementHeight - 1.5f * 2.0f};

        // Custom objects
            // Mesh
        rectMesh = Mesh::CreateRectMesh(1.0f, 1.0f);
        triangleMesh = Mesh::CreateTriangleMesh({0.0f, -0.5f}, {0.5f, 0.5f}, {-0.5f, 0.5f});
        checkmarkMesh = Mesh::CreateCheckmarkMesh(1.0f, 1.0f);

            // Shaders
        primitiveShader = Shader::Create("Anwill/res/shaders/OpenGL/GuiPrimitiveColor.glsl");
        circleShader = Shader::Create("Anwill/res/shaders/OpenGL/GuiCircleColor.glsl");
        GuiStyling::primitiveShader->Bind();
        GuiStyling::primitiveShader->SetUniformVec3f({1.0f, 1.0f, 1.0f}, "u_Color");
        GuiStyling::primitiveShader->Unbind();
        Text::shader = Shader::Create("Anwill/res/shaders/OpenGL/GuiText.glsl");
        Button::shaders[(std::size_t) Button::Shape::Rectangle] = Shader::Create("Anwill/res/shaders/OpenGL/GuiRectButton.glsl");
        Button::shaders[(std::size_t) Button::Shape::Ellipse] = Shader::Create("Anwill/res/shaders/OpenGL/GuiCircleButton.glsl");
        Window::shader = Shader::Create("Anwill/res/shaders/OpenGL/GuiWindow.glsl");
        Tooltip::shader = Shader::Create("Anwill/res/shaders/OpenGL/GuiTooltip.glsl");

            // Fonts
        Text::font = std::make_unique<Font>("Sandbox/assets/fonts/arial.ttf");
        Text::font->SetNewlineSpace(1.4f);
    }
}