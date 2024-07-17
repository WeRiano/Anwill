#pragma once

#include <functional>

#include "core/Core.h"
#include "math/Vec2f.h"
#include "gfx/Mesh.h"
#include "gfx/Shader.h"
#include "gfx/Font.h"

/// Use the given style of type StyleType if it exists (this will most likely include an upcast),
/// otherwise create a new instance of it
#define AW_GUI_ENSURE_STYLE(style, StyleType) style == nullptr ? Anwill::CreateShared<StyleType>() : style

/// Downcast the style to a derived StyleType.
#define AW_GUI_CAST_STYLE(style, StyleType) std::static_pointer_cast<StyleType>(style)

namespace Anwill {

    /**
     * Colors, margins, padding, shaders, meshes, etc. Anything that alters the look of the GUI.
     * Static and non-static members represent global and non-global styling.
     * Non-const members can be altered by the client, const members are meant to be unchanged.
     */
    struct GuiStyling
    {
    public:
        // --- Styling of shared elements (icons, scrollbar, etc.) ---
        static Mesh rectMesh, triangleMesh, checkmarkMesh;
        static std::shared_ptr<Shader> primitiveShader, circleShader;

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
            // The amount of space from the 'middle' (baseline) where the text should be rendered
            static float baselineOffset;
            static std::unique_ptr<Font> font;
            static std::shared_ptr<Shader> shader;
        };

        struct Button {
        public:
            enum class Shape : short {
                Rectangle = 0,
                Ellipse,

                Size
            };

            // Index with Button::Shape
            static std::array<std::shared_ptr<Shader>, (size_t) Shape::Size> shaders;

            Shape buttonShape = Shape::Rectangle;
            Math::Vec3f buttonColor = {0.15f, 0.15f, 0.6f};
            Math::Vec3f buttonHoverColor = {0.20f, 0.20f, 0.95f};
            Math::Vec3f buttonPressColor = {0.28f, 0.28f, 1.00f};
        };

        // Text button
        struct TextButton : public Text, public Button {
        public:
            static float textPadding; // X distance from button edge to text
        };

        struct Checkbox : public Text, public Button /* public Text, public Button */ {
        public:
            enum class CheckmarkType : short {
                Tick = 0,
                Rectangle,
                Ellipse,
                // TODO: Cross

                Size
            };

            static float textMargin; // Distance between the checkbox button and the text
            static float iconMargin; // Distance between icon and the edge of the box

            CheckmarkType checkmarkType = CheckmarkType::Tick;
            Math::Vec3f checkmarkColor = {0.35f, 0.45f, 1.0f};
        };

        struct RadioButton : public Text, public Button {
        public:
            Math::Vec3f checkmarkColor = {0.35f, 0.45f, 1.0f};
        };

        struct Slider : public Text, public Button {
        public:
            static float markerWidth;

            Math::Vec3f markerColor = {0.38f, 0.45f, 1.0f};

            static inline Math::Vec2f GetMarkerSize(float rowHeight) {
                return {markerWidth, rowHeight - 3.0f};
            }
        };

        struct InputText : public Text, public Button {
        public:
            static std::shared_ptr<VertexArray> cursorVertexArray;
            static unsigned int cursorShowTimeIntervalMS;

            Math::Vec3f selectedTextHighlightColor = {0.30f, 0.38f, 1.0f};
            static inline float GetCursorHeight(float rowHeight) {
                return rowHeight - 8.0f;
            }
        };

        struct Image {
        public:
            static std::shared_ptr<Shader> shader;
        };

        // Used to prevent creation of duplicate members
        struct Container {
        public:
            /// Initial horizontal spacing
            float elementIndent;
            float elementHeight;
            /// Space between neighbouring elements
            float elementVerticalMargin, elementHorizontalMargin;
            /// Additional empty window space (scissor box padding)
            float edgeCutoffPadding;
            float scrollbarWidth; // TODO: Nested struct for scrollbar settings
            Math::Vec3f iconColor;

            inline Math::Vec2f GetFirstElementPos() const {
                return {elementIndent, -GetRowHeight()};
            }

            inline float GetRowHeight() const {
                return elementHeight + elementVerticalMargin;
            }

            inline Math::Vec2f GetIconSize() const {
                return {elementHeight, elementHeight};
            }

        protected:
            Container(float elementIndent, float elementHeight, float elementVerticalMargin,
                      float elementHorizontalMargin, float edgeCutoffPadding)
                : elementIndent(elementIndent), elementHeight(elementHeight),
                  elementVerticalMargin(elementVerticalMargin),
                  elementHorizontalMargin(elementHorizontalMargin), edgeCutoffPadding(edgeCutoffPadding),
                  scrollbarWidth(10.0f), iconColor(1.0f, 1.0f, 1.0f)
            {}
        };

        struct Dropdown : public TextButton, public Container {
        public:

            Dropdown()
                : Container(10.0f, 30.0f, 5.0f, 6.0f,
                            4.0f)
            {}
        };

        struct Window : public Container {
        public:
            static float borderSize, headerSize;
            static unsigned int titleFontSize;
            static std::shared_ptr<Shader> shader;

            Window()
                : Container(5.0f, 30.0f, 5.0f, 6.0f,
                            4.0f)
            {}

            inline Math::Vec2f GetTitlePos() {
                return {borderSize + GetIconSize().X,
                        elementHeight / 2.0f - headerSize / 2.0f};
            }
        };

        static void InitGlobalStyling();
    };
}
