#include "GuiWindow.h"
#include "gui/elements/GuiIcon.h"
#include "math/Algo.h"
#include "gfx/Renderer.h"
#include "gfx/Renderer2D.h"

namespace Anwill {

    GuiWindow::GuiWindow(const std::string& title, const Math::Vec2f& position, const Math::Vec2f& size)
        : GuiContainer(std::make_shared<GuiStyling::Window>(), true, 0),
          m_Style(std::static_pointer_cast<GuiStyling::Window>(GuiContainer::m_Style)),
          m_Pos(position), m_Size(size), m_LastShowSize(),
          m_Title(m_Style, title),
          m_MinimizeButton(std::make_shared<GuiButton>(m_Style, m_Style->GetIconSize(),
                                                       [this]() {
                                                           ToggleElementsVisibility();
                                                           if(IsShowingElements()) {
                                                               m_Size = m_LastShowSize;
                                                           } else {
                                                               m_LastShowSize = m_Size;
                                                               m_Size.Y = m_Style->headerSize;
                                                           }
                                                       }))
    {
        m_Title.m_Style->fontSize = m_Style->titleFontSize;
    }

    std::shared_ptr<GuiElement> GuiWindow::GetHoverElement(Math::Vec2f& hoverElementPos,
                                                           const Math::Vec2f& mousePos) const
    {
        // Two things in addition to GuiContainer implementation:
        //  1. We have a static minimize button element that we have to check for hovering
        //  2. We have to consider the position of the window since it is the "outer" most container
        if(m_MinimizeButton->IsHovering(mousePos - m_Pos)) {
            return m_MinimizeButton;
        }
        auto hoverElement = GuiContainer::GetHoverElement(hoverElementPos,
                                                          mousePos - m_Pos);
        hoverElementPos = m_Pos + hoverElementPos;
        return hoverElement;
    }

    void GuiWindow::Render(bool isSelected, const Timestamp& delta)
    {
        Renderer::PushScissor({m_Pos.X, m_Pos.Y - m_Size.Y}, m_Size);
        // Render window
        RenderBackground(isSelected);
        // Render title
        RenderTitle(delta);
        // Render minimize button
        m_MinimizeButton->Render(m_Pos, m_Size, delta);

        if(IsShowingElements()) {
            // Render icon
            GuiIcon::RenderDownArrow(m_Pos, m_Style->GetIconSize() * 0.5f, m_Style->iconColor);

            // Render contained elements
            GuiContainer::Render(m_Pos + m_Style->GetFirstElementPos(),
                                 m_Size - m_Style->GetFirstElementPos().NegateY()
                                 - Math::Vec2f(m_Style->edgeCutoffPadding, m_Style->edgeCutoffPadding),
                                 delta);

            // Render scrollbar
            Math::Vec2f scrollBarPos = {m_Pos.X + m_Size.X - m_Style->scrollbarWidth - 3.0f,
                                        m_Pos.Y + -GuiStyling::Window::headerSize - 4.0f};
            GuiContainer::RenderVerticalScrollbar(scrollBarPos, m_Size.Y - GuiStyling::Window::headerSize,
                                                  delta);
        } else {
            // Render icon
            GuiIcon::RenderRightArrow(m_Pos,
                                      m_Style->GetIconSize() * 0.5f,
                                      m_Style->iconColor);
        }
        Renderer::EndScissor();
    }

    bool GuiWindow::IsHoveringHeader(const Math::Vec2f& mousePos)
    {
        auto titlePos = m_Style->GetTitlePos();
        return Math::Algo::IsPointInsideRectangle({m_Pos.X + titlePos.X, m_Pos.Y},
                                                  {m_Pos.X + m_Size.X, m_Pos.Y},
                                                  {m_Pos.X + m_Size.X, m_Pos.Y - GuiStyling::Window::headerSize},
                                                  {m_Pos.X + titlePos.X, m_Pos.Y - GuiStyling::Window::headerSize},
                                                  mousePos);
    }

    bool GuiWindow::IsHoveringResize(const Math::Vec2f& mousePos)
    {
        return Math::Algo::IsPointInsideCircle({m_Pos.X + m_Size.X, m_Pos.Y - m_Size.Y},
                                               10.0f, mousePos);
    }

    bool GuiWindow::IsHoveringWindow(const Math::Vec2f& mousePos)
    {
        return Math::Algo::IsPointInsideRectangle({m_Pos.X, m_Pos.Y},
                                                  {m_Pos.X + m_Size.X, m_Pos.Y},
                                                  {m_Pos.X + m_Size.X, m_Pos.Y - m_Size.Y},
                                                  {m_Pos.X, m_Pos.Y - m_Size.Y},
                                                  mousePos);
    }

    bool GuiWindow::IsHoveringMinimize(const Math::Vec2f& mousePos) {
        return Math::Algo::IsPointInsideRectangle({m_Pos.X, m_Pos.Y},
                                                  {m_Pos.X + m_Style->GetIconSize().X, m_Pos.Y},
                                                  {m_Pos.X + m_Style->GetIconSize().X, m_Pos.Y
                                                                                       - GuiStyling::Window::headerSize},
                                                  {m_Pos.X, m_Pos.Y - GuiStyling::Window::headerSize},
                                                  mousePos);
    }

    void GuiWindow::Move(const Math::Vec2f& delta, const Math::Vec2f& minPos, const Math::Vec2f& maxPos)
    {
        if(!(m_Pos.Y + delta.Y > maxPos.Y ||
             m_Pos.Y - GuiStyling::Window::headerSize + delta.Y < minPos.Y)) {
            m_Pos = {m_Pos.X, m_Pos.Y + delta.Y};
        }
        if(!(m_Pos.X + delta.X < minPos.X ||
             m_Pos.X + m_Size.X + delta.X > maxPos.X)) {
            m_Pos = {m_Pos.X + delta.X, m_Pos.Y};
        }
    }

    void GuiWindow::Resize(const Math::Vec2f& delta, const Math::Vec2f& minSize, const Math::Vec2f& maxSize)
    {
        Math::Vec2f newSize = m_Size + delta;
        newSize.Clamp(minSize, maxSize);
        m_Size = newSize;

        GuiContainer::AdjustScrollOnResize(delta);
    }

    void GuiWindow::ScrollUp()
    {
        GuiContainer::Scroll(-10.0f);
    }

    void GuiWindow::ScrollDown()
    {
        GuiContainer::Scroll(10.0f);
    }

    Math::Vec2f GuiWindow::GetPos() const
    {
        return m_Pos;
    }

    void GuiWindow::RenderBackground(bool isSelected)
    {
        GuiStyling::Window::shader->Bind();
        GuiStyling::Window::shader->SetUniform1i(isSelected, "u_Selected");
        auto transform = Math::Mat4f::Translate(Math::Mat4f::Identity(),
                                                m_Pos + Math::Vec2f(m_Size.X / 2.0f, -m_Size.Y / 2.0f));
        transform = Math::Mat4f::Scale(transform, m_Size);
        Renderer2D::SubmitMesh(GuiStyling::Window::shader, GuiStyling::rectMesh, transform);
    }

    void GuiWindow::RenderTitle(const Timestamp& delta)
    {
        auto style = std::dynamic_pointer_cast<GuiStyling::Window>(m_Style);
        m_Title.Render(m_Pos + std::dynamic_pointer_cast<GuiStyling::Window>(m_Style)->GetTitlePos(),
                       m_Size - m_Style->GetFirstElementPos() - Math::Vec2f(m_Style->edgeCutoffPadding,
                                                                            m_Style->edgeCutoffPadding),
                       delta);
    }
}