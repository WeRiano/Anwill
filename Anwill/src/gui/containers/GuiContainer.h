#pragma once

#include <memory>

#include "core/Core.h"
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
        Shared<GuiElement> element;
        Math::Vec2f position;
        bool onNewRow, forceNextToNewRow;
        bool isHidden;

        ContainerElement(const Shared<GuiElement>& element, Math::Vec2f position,
                         bool onNewRow, bool forceNextToNewRow, bool isHidden)
            : element(element), position(position), onNewRow(onNewRow),
              forceNextToNewRow(forceNextToNewRow), isHidden(isHidden)
        {}
    };

    class GuiContainer {
    public:
        Shared<GuiStyling::Container> m_Style;

        GuiContainer(const Shared<GuiStyling::Container>& style, bool showElements,
                     unsigned int gridDepth = 0);

        /**
         * Get the current element that is being hovered by the mouse cursor, if any.
         * @param hoverElementPos The position of the hovered element. Ignore if result is null.
         * @param mousePos Position of the mouse cursor.
         * @return The element currently being hovered or a nullptr if there isn't one.
         */
        virtual Shared<GuiElement> GetHoverElement(Math::Vec2f& hoverElementPos,
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
        Shared<E> AddElement(bool onNewRow, bool forceNextToNewRow, Args&&... args)
        {
            m_ContainerElements.emplace_back(std::make_shared<E>(m_Style, std::forward<Args>(args)...),
                                             Math::Vec2f(), onNewRow, forceNextToNewRow, false);
            if(m_ContainerElements.empty() || (onNewRow || m_ContainerElements.back().forceNextToNewRow))
            {
                m_GridDepth += m_ContainerElements.back().element->GetGridDepth();
            }
            return std::dynamic_pointer_cast<E>(m_ContainerElements.back().element);
        }

        void RemoveElement(const Shared<GuiElement>& element);
        void RemoveElements(const Shared<GuiElement>& element, unsigned int additionalElements = 0);
        void Clear();

    protected:
        std::vector<ContainerElement> m_ContainerElements;
        GuiButton m_Scrollbar;
        Math::Vec2f m_ScrollOffset, m_HiddenSize;
        bool m_CanScroll;
        unsigned int m_GridDepth;
        volatile bool m_ShowElements; // TODO: Why volatile?

        /**
         * @brief Get the maximum allowed width and height of an element given its position and
         *        the current maximum width and height.
         */
        Math::Vec2f GetNextElementSize(const Math::Vec2f& posDelta, const Math::Vec2f& oldMaxSize);
        /**
         * @brief Get the position of the next horizontal or vertical element in a container
         */
        Math::Vec2f GetNextElementPos(const Math::Vec2f& elementPosition, float elementWidth,
                                      unsigned int elementGridDepth, float newRowXPos, bool onNewRow);
        void RenderDebugGrid(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize);
    };
}