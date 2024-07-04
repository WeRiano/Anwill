#pragma once

#include "GuiTooltip.h"
#include "gui/styles/GuiStyling.h"
#include "core/Timestamp.h"
#include "math/Vec2f.h"
#include "core/KeyCodes.h"

/// Use the given style of type StyleType if it exists (this will most likely include an upcast),
/// otherwise create a new instance of it
#define AW_GUI_MAKE_STYLE(style, StyleType) style == nullptr ? std::make_shared<StyleType>() : style
/// Downcast the style to a derived StyleType.
#define AW_GUI_CAST_STYLE(style, StyleType) std::static_pointer_cast<StyleType>(style)

namespace Anwill {

    class GuiElement
    {
    public:
        GuiElement(const std::shared_ptr<GuiStyling::Container>& containerStyle);

        virtual void Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                            const Timestamp& delta) = 0;
        /**
         * @brief Check if the mouse cursor is hovering the element
         */
        virtual bool IsHovering(const Math::Vec2f& mousePos) const = 0;
        /**
         * The amount of horizontal space the element occupies starting from its assigned position.
         * This includes any internal padding and/or margins.
         */
        virtual float GetWidth() const = 0;
        virtual unsigned int GetGridDepth() const = 0;

        virtual void StartHovering();
        virtual void OnHover(const Math::Vec2f& mousePos);
        virtual void StopHovering();
        virtual void StartPressing();
        virtual void OnPress(const Math::Vec2f& mousePos);
        virtual void Release();
        virtual void Select();
        virtual void Deselect();
        virtual void OnKeyPress(const KeyCode& keyCode);
        virtual void OnKeyRepeat(const KeyCode& keyCode);
        virtual void OnKeyRelease(const KeyCode& keyCode);
        virtual void OnKeyChar(unsigned char c);
        virtual void OnHoverRender(const Math::Vec2f& mousePos, const Math::Vec2f& gameWindowSize);

        void EmplaceTooltip(const std::string& tooltipText);

    protected:
        bool m_IsHovered, m_IsPressed, m_IsSelected;
        std::unique_ptr<GuiTooltip> m_Tooltip;
        // Style of whatever container is hosting this element
        std::shared_ptr<GuiStyling::Container> m_HostContainerStyle;
    };
}