#pragma once

#include <memory>

#include "gui/elements/GuiElement.h"
#include "gui/elements/GuiText.h"
#include "gui/elements/GuiButton.h"
#include "gui/styles/GuiStyling.h"
#include "gfx/Texture.h"
#include "math/Vec2f.h"
#include "core/Timestamp.h"

namespace Anwill {

    struct ContainerElement {
    public:
        std::shared_ptr<GuiElement> element;
        Math::Vec2f position;
        bool onNewRow, forceNextToNewRow;
        bool isHidden;

        ContainerElement(const std::shared_ptr<GuiElement>& element, Math::Vec2f position,
                         bool onNewRow, bool forceNextToNewRow, bool isHidden)
            : element(element), position(position), onNewRow(onNewRow),
              forceNextToNewRow(forceNextToNewRow), isHidden(isHidden)
        {}
    };

    class GuiContainer {
    public:
        std::shared_ptr<GuiStyling::Container> m_Style;

        GuiContainer(const std::shared_ptr<GuiStyling::Container>& style, bool showElements,
                     unsigned int gridDepth = 0);

        /**
         * Get the current element that is being hovered by the mouse cursor, if any.
         * @param hoverElementPos The position of the hovered element. Ignore if result is null.
         * @param mousePos Position of the mouse cursor.
         * @return The element currently being hovered or a nullptr if there isn't one.
         */
        virtual std::shared_ptr<GuiElement> GetHoverElement(Math::Vec2f& hoverElementPos,
                                                            const Math::Vec2f& mousePos) const;
        void Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                    const Timestamp& delta);
        void RenderVerticalScrollbar(const Math::Vec2f& assignedPos, float visibleHeight, const Timestamp& delta);
        bool IsShowingElements() const;
        void ToggleElementsVisibility();
        /**
         * Scroll the container. A negative delta value will scroll down and vice-versa.
         */
        void Scroll(float delta);
        void AdjustScrollOnResize(const Math::Vec2f& resizeDelta);
        unsigned int GetGridDepth() const;

        template <class E, typename... Args>
        std::shared_ptr<E> AddElement(bool onNewRow, bool forceNextToNewRow, Args&&... args)
        {
            m_ContainerElements.emplace_back(std::make_shared<E>(m_Style, std::forward<Args>(args)...),
                                             Math::Vec2f(), onNewRow, forceNextToNewRow, false);
            if(m_ContainerElements.empty() || (onNewRow || m_ContainerElements.back().forceNextToNewRow))
            {
                m_GridDepth += m_ContainerElements.back().element->GetGridDepth();
            }
            return std::dynamic_pointer_cast<E>(m_ContainerElements.back().element);
        }

    protected:
        std::vector<ContainerElement> m_ContainerElements;
        GuiButton m_Scrollbar;
        Math::Vec2f m_ScrollOffset, m_HiddenSize;
        bool m_CanScroll;
        unsigned int m_GridDepth;
        volatile bool m_ShowElements; // TODO: Why volatile?

        Math::Vec2f GetNextElementSize(const Math::Vec2f& posDelta, const Math::Vec2f& oldMaxSize);
        Math::Vec2f GetNextElementPos(const Math::Vec2f& elementPosition, float elementWidth,
                                      unsigned int elementGridDepth, float newRowXPos, bool onNewRow);
    };
}