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