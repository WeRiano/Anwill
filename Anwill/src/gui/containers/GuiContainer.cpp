#include "GuiContainer.h"
#include "gfx/Renderer.h"
#include "math/Math.h"
#include "gui/elements/GuiIcon.h"

namespace Anwill {

    GuiContainer::GuiContainer(const std::shared_ptr<GuiStyling::Container>& style, bool showElements,
                               unsigned int gridDepth)
        : m_Style(style), m_ContainerElements(),
          m_Scrollbar(style, {m_Style->scrollbarWidth, 10.0f}, []() {}),
          m_ScrollOffset(), m_HiddenSize(), m_CanScroll(false), m_GridDepth(gridDepth), m_ShowElements(showElements)
    {
        m_Scrollbar.m_Style->buttonColor = {0.3f, 0.3f, 0.3f}; // TODO: Move
    }

    std::shared_ptr<GuiElement> GuiContainer::GetHoverElement(Math::Vec2f& hoverElementPos,
                                                              const Math::Vec2f& mousePos) const
    {
        auto adjustedMousePos = mousePos - m_Style->GetFirstElementPos();
        if (!m_ShowElements) { return nullptr; }
        for (const auto& containerElement : m_ContainerElements)
        {
            if (containerElement.element->IsHovering( adjustedMousePos - containerElement.position))
            {
                hoverElementPos = containerElement.position;
                return containerElement.element;
            }
            if (dynamic_cast<GuiContainer*>(containerElement.element.get()) != nullptr)
            {
                // If element is a container we need to check with those elements
                auto container =
                    std::dynamic_pointer_cast<GuiContainer>(containerElement.element);
                Math::Vec2f elementPosInsideContainer = {};
                auto maybeResult = container->GetHoverElement(
                    elementPosInsideContainer, adjustedMousePos - containerElement.position);
                if (maybeResult != nullptr)
                {
                    // If we found an element we return it, otherwise we continue
                    hoverElementPos = containerElement.position + elementPosInsideContainer;
                    return maybeResult;
                }
            }
        }
        return nullptr;
    }

    void GuiContainer::Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                              const Timestamp& delta)
    {
        if(!m_ShowElements) { return; }
        Renderer::PushScissor({assignedPos.X, assignedPos.Y - assignedMaxSize.Y}, assignedMaxSize);
        Math::Vec2f elementPos = m_ScrollOffset;
        float newRowXPos = elementPos.X;
        std::shared_ptr<GuiElement> lastElement = nullptr;
        for (unsigned int i = 0; i < m_ContainerElements.size(); i++)
        {
            auto& containerElement = m_ContainerElements[i];
            if (i > 0)
            {
                // Any element other than the first needs to know if it's on a new row or not
                // and then have its position calculated
                bool wantsNewRow = m_ContainerElements[i].onNewRow;
                // wrong? Should be bool from prev element
                bool forcedToNewRow = m_ContainerElements[i - 1].forceNextToNewRow;
                elementPos = GetNextElementPos(elementPos,
                                               lastElement->GetWidth(),
                                               lastElement->GetGridDepth(),
                                               newRowXPos,
                                               wantsNewRow || forcedToNewRow);
            }
            // Only render the element if it is inside the
            if (elementPos.Abs() >= assignedMaxSize)
            {
                m_ContainerElements[i].isHidden = true;
            } else
            {
                m_ContainerElements[i].isHidden = false;
                containerElement.element->Render(assignedPos + elementPos,
                                                 GetNextElementSize(elementPos, assignedMaxSize),
                                                 delta);
            }
            containerElement.position = elementPos;
            lastElement = containerElement.element;
        }
        Renderer::EndScissor();
    }

    void GuiContainer::RenderVerticalScrollbar(const Math::Vec2f& assignedPos, float visibleHeight,
                                               const Timestamp& delta)
    {
        float contentHeight = (float) m_GridDepth * m_Style->GetRowHeight();
        // How much of the window is hidden due to content overflow
        float hiddenHeight = Math::Max(contentHeight - visibleHeight, 0.0f);
        m_HiddenSize.Y = hiddenHeight;
        if (hiddenHeight <= 0.0f)
        {
            // Can only scroll if there is some hidden content
            m_CanScroll = false;
            return;
        }

        m_CanScroll = true;
        float scrollbarHeight = (visibleHeight * visibleHeight) / contentHeight;
        float emptyHeight = (visibleHeight - scrollbarHeight);
        // This coefficient represents how "far" the user has scrolled into hidden territory (and will be on [0, 1])
        float scrollDistNorm = m_ScrollOffset.Y / m_HiddenSize.Y;
        Math::Vec2f scrollbarPosOffset = Math::Vec2f(0.0f, emptyHeight * scrollDistNorm);
        m_Scrollbar.SetHeight(scrollbarHeight);
        // TODO: Max size unused
        m_Scrollbar.Render(assignedPos - scrollbarPosOffset, {9999.9f, 9999.9f},
                           delta);
    }

    bool GuiContainer::IsShowingElements() const
    {
        return m_ShowElements;
    }

    void GuiContainer::ToggleElementsVisibility()
    {
        m_ShowElements = !m_ShowElements;
    }

    void GuiContainer::Scroll(float delta)
    {
        // TODO: Horizontal scrolling
        if (!m_CanScroll)
        {
            return;
        }
        // TODO: Adjust for cutoff padding
        m_ScrollOffset.Y = Math::Clamp(m_ScrollOffset.Y + delta, 0.0f, m_HiddenSize.Y);
    }

    void GuiContainer::AdjustScrollOnResize(const Math::Vec2f& resizeDelta)
    {
        // TODO: Horizontal scrolling
        if (m_ScrollOffset.Y >= 0.0f && m_HiddenSize.Y != 0.0f && m_HiddenSize.Y <= m_ScrollOffset.Y)
        {
            m_ScrollOffset.Y = Math::Max(m_ScrollOffset.Y - resizeDelta.Y, 0.0f);
        }
    }

    unsigned int GuiContainer::GetGridDepth() const
    {
        return m_GridDepth;
    }

    void GuiContainer::RemoveElement(const Shared<GuiElement>& element)
    {
        for(int i = 0; i < m_ContainerElements.size(); i++)
        {
            if(m_ContainerElements[i].element == element)
            {
                m_ContainerElements.erase(m_ContainerElements.begin() + i);
                m_GridDepth--;
                break;
            }
        }
    }

    void GuiContainer::RemoveElements(const Shared<GuiElement>& element, unsigned int additionalElements)
    {
        for(int i = 0; i < m_ContainerElements.size(); i++)
        {
            if(m_ContainerElements[i].element == element)
            {
                additionalElements = Math::Min(additionalElements,
                                               (unsigned int)(m_ContainerElements.size() - i));
                m_ContainerElements.erase(m_ContainerElements.begin() + i,
                                          m_ContainerElements.begin() + i + additionalElements);
                m_GridDepth -= additionalElements + 1;
                break;
            }
        }
    }

    void GuiContainer::Clear()
    {
        m_GridDepth = 0;
        m_ContainerElements.clear();
    }

    Math::Vec2f GuiContainer::GetNextElementSize(const Math::Vec2f& posDelta, const Math::Vec2f& oldMaxSize)
    {
        return {oldMaxSize.X - posDelta.X, oldMaxSize.Y + posDelta.Y};
    }

    Math::Vec2f GuiContainer::GetNextElementPos(const Math::Vec2f& elementPosition, float elementWidth,
                                                unsigned int elementGridDepth, float newRowXPos, bool onNewRow)
    {
        if (onNewRow)
        {
            return {
                newRowXPos,
                elementPosition.Y - ((float) elementGridDepth *
                                        (m_Style->elementHeight + m_Style->elementVerticalMargin))
            };
        } else
        {
            return {
                elementPosition.X + elementWidth + m_Style->elementHorizontalMargin,
                elementPosition.Y
            };
        }
    }

    void GuiContainer::RenderDebugGrid(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize)
    {
        Math::Vec2f elementSpacePos = assignedPos;
        Math::Vec2f elementSpaceSize = {assignedMaxSize.X, m_Style->elementHeight};
        Math::Vec2f marginSpacePos = elementSpacePos;
        marginSpacePos.Y -= elementSpaceSize.Y;
        Math::Vec2f marginSpaceSize = {assignedMaxSize.X, m_Style->elementVerticalMargin};
        unsigned int gridDepth = 1;
        if(m_ShowElements)
        {
            gridDepth = GetGridDepth();
        }
        for(int i = 0; i < gridDepth; i++) {
            GuiIcon::RenderRectangle(elementSpacePos, elementSpaceSize,
                                     {1.0f, 0.1f, 0.1f});
            GuiIcon::RenderRectangle(marginSpacePos, marginSpaceSize,
                                     {0.01f, 0.9f, 0.85f});
            elementSpacePos.Y -= m_Style->GetRowHeight();
            marginSpacePos.Y -= m_Style->GetRowHeight();
        }
    }
}