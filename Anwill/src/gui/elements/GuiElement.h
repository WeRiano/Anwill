#pragma once

#include "GuiTooltip.h"
#include "gui/styles/GuiStyling.h"
#include "core/Timestamp.h"
#include "math/Vec2f.h"
#include "core/KeyCodes.h"

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
         * This includes any internal indentation, padding and/or margins.
         */
        virtual float GetWidth() const = 0;
        /**
         * @return The number of rows the element occupies in the containers grid.
         */
        virtual unsigned int GetGridDepth() const = 0;
        /**
         * Invoked only once when the element becomes hovered.
         */
        virtual void StartHovering();
        /**
         * Invoked only once when the element stops being hovered.
         */
        virtual void StopHovering();
        /**
         * Invoked when the element is being hovered, which may be repeated.
         */
        virtual void OnHover(const Math::Vec2f& mousePos);
        /**
         * Invoked only once when the element becomes pressed.
         */
        virtual void StartPressing();
        /**
         * Invoked when the element is being pressed, which may be repeated.
         */
        virtual void OnPress(const Math::Vec2f& mousePos);
        /**
         * Invoked only once when the element stops being pressed.
         */
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