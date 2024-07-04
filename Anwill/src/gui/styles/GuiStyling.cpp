#include "GuiStyling.h"
#include "gfx/ShaderMacros.h"

namespace Anwill {

    float GuiStyling::Window::headerSize,
    GuiStyling::Text::baselineOffset, GuiStyling::TextButton::textPadding, GuiStyling::Checkbox::iconMargin,
    GuiStyling::Window::borderSize, GuiStyling::Tooltip::borderSize, GuiStyling::Tooltip::offset,
    GuiStyling::Checkbox::textMargin, GuiStyling::Text::cursorHeight, GuiStyling::Slider::markerWidth;

    unsigned int GuiStyling::Text::fontSize, GuiStyling::Text::cursorShowTimeIntervalMS,
    GuiStyling::Window::titleFontSize;

    Math::Vec2f GuiStyling::Tooltip::windowMargin;

    std::unique_ptr<Font> GuiStyling::Text::font;

    std::shared_ptr<Shader> GuiStyling::Text::shader, GuiStyling::primitiveShader, GuiStyling::circleShader,
    GuiStyling::Window::shader,
    GuiStyling::Tooltip::shader, GuiStyling::Image::shader;

    //const std::shared_ptr<Shader> GuiStyling::Window::shader;

    std::shared_ptr<VertexArray> GuiStyling::Text::cursorVertexArray;

    Mesh GuiStyling::checkmarkMesh, GuiStyling::triangleMesh, GuiStyling::rectMesh;

    std::array<std::shared_ptr<Shader>, (size_t) GuiStyling::Button::Shape::Size> GuiStyling::Button::shaders;

    void GuiStyling::InitGlobalStyling()
    {
        // --- Window ---
        Window::borderSize = 8.0f;
        Window::headerSize = 30.0f;
        Window::titleFontSize = 14;
        ShaderMacros::SetMacro("AW_GUI_WINDOW_BORDER_SIZE", Window::borderSize);
        ShaderMacros::SetMacro("AW_GUI_WINDOW_HEADER_SIZE", Window::headerSize);

        // --- Tooltip ---
        Tooltip::offset = 15.0f;
        Tooltip::windowMargin = {10.0f, 10.0f};
        Tooltip::borderSize = 2.0f;
        ShaderMacros::SetMacro("AW_GUI_TOOLTIP_BORDER_SIZE", Tooltip::borderSize);

        // --- Text ---
        Text::fontSize = 13; // TODO: Attempt to change this and see if baselineOffset sticks
        Text::baselineOffset = -(float) Text::fontSize * 0.45f;
        TextButton::textPadding = 5.0f; // X distance from button edge to text
        Text::cursorVertexArray = VertexArray::Create();
        //Text::cursorHeight = Window::elementHeight - 5.0f;
        Text::cursorShowTimeIntervalMS = 750;

        float unitVerticalLineVertices[] = {
            0.0f, 0.5f,
            0.0f, -0.5f
        };
        auto tempVB = VertexBuffer::Create(unitVerticalLineVertices, sizeof(unitVerticalLineVertices));
        std::vector<BufferElement> elements = {
                Anwill::BufferElement(Anwill::ShaderDataType::Float2)
        };
        auto bufferLayout = Anwill::BufferLayout(elements);
        Text::cursorVertexArray->AddBuffer(*tempVB, bufferLayout);

        // --- Checkbox ---
        Checkbox::textMargin = 5.0f;
        Checkbox::iconMargin = 5.0f;

        // --- Dropdown --

        // --- Slider ---
        //Slider::markerSize = {13.0f, Window::elementHeight - 1.5f * 2.0f};
        Slider::markerWidth = 13.0f;

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
        Image::shader = Shader::Create("Anwill/res/shaders/OpenGL/GuiTexture.glsl");

            // Fonts
        Text::font = std::make_unique<Font>("Sandbox/assets/fonts/arial.ttf");
        Text::font->SetNewlineSpace(1.4f);
    }
}