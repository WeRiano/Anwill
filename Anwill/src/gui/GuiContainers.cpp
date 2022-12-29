#include "gui/GuiContainers.h"
#include "math/Algo.h"

namespace Anwill {

    std::shared_ptr<Shader> GuiWindow::s_WindowShader;
    const Math::Vec2f GuiWindow::s_TitlePos = {GuiMetrics::WindowBorderSize + s_IconWidthHeight,
                               GuiMetrics::WindowElementHeight / 2.0f - GuiMetrics::WindowHeaderSize / 2.0f};
    const Math::Vec2f GuiWindow::s_MinimizeIconPos = {GuiMetrics::WindowBorderSize / 2.0f, 0.0f};

    GuiContainer::GuiContainer()
        : m_GridDepth(1), m_HideElements(true)
    {}

    std::shared_ptr<GuiElement> GuiContainer::GetHoverElementInternal(const Math::Vec2f& mousePos,
                                                                      const Math::Vec2f& posOffset) const
    {
        for(unsigned int i = 0; i < m_Elements.size(); i++) {
            auto& element = m_Elements[i];
            if(element->IsHovering(mousePos - (posOffset + m_ElementPosCache[i]))) {
                return element;
            }
            if(dynamic_cast<GuiContainer*>(element.get()) != nullptr) {
                // If element is a container we need to check with those elements
                auto container = std::dynamic_pointer_cast<GuiContainer>(element);
                if(!container->m_HideElements) {
                    auto maybeResult = container->GetHoverElement(mousePos - (posOffset + m_ElementPosCache[i]));
                    if (maybeResult != nullptr)
                    {
                        // If we found an element we return it, otherwise we continue
                        return maybeResult;
                    }
                }
            }
        }
        return nullptr;
    }

    void GuiContainer::Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                              const Math::Vec2f& firstPos)
    {
        m_ElementPosCache.clear();
        Math::Vec2f elementGridPos = firstPos;
        float newRowXPos = elementGridPos.GetX();
        std::shared_ptr<GuiElement> lastElement = nullptr;
        for (unsigned int i = 0; i < m_Elements.size(); i++) {
            auto element = m_Elements[i];
            if(i > 0) {
                bool wantsNewRow = element->OnNewRow();
                bool forcedToNewRow = lastElement->ForceNextToNewRow();
                elementGridPos = GuiMetrics::GetNextElementPos(elementGridPos, lastElement->GetWidth(),
                                                               lastElement->GetGridDepth(),
                                                               newRowXPos,
                                                               wantsNewRow || forcedToNewRow);
            }
            element->Render(assignedPos + elementGridPos, { assignedMaxSize.GetX() - elementGridPos.GetX() - GuiMetrics::WindowCutoffMargin,
                                                      assignedMaxSize.GetY() + elementGridPos.GetY() - GuiMetrics::WindowCutoffMargin });
            m_ElementPosCache.push_back(elementGridPos);
            lastElement = element;
        }
    }

    bool GuiContainer::IsHidingElements() const
    {
        return m_HideElements;
    }

    GuiDropdown::GuiDropdown(const std::string& text, unsigned int textSize)
            : GuiTextButton(true, text, textSize, [this](){
                m_HideElements = !m_HideElements;
            })
    {
        // Can't place stuff to the right of a dropdown
        m_ForceNextToNewRow = true;
        // Give space for arrow icon
        m_TextPos += {s_IconWidthHeight, 0.0f};
    }

    void GuiDropdown::Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize)
    {
        // Force button size to max width render it
        m_ButtonSize = { assignedMaxSize.GetX(), m_ButtonSize.GetY()};
        GuiTextButton::Render(assignedPos, assignedMaxSize);

        // Render arrow icon
        Math::Mat4f iconTransform = Math::Mat4f::Scale(Math::Mat4f::Identity(), {s_IconWidthHeight * 0.5f, s_IconWidthHeight * 0.5f, 0.0f});
        if(m_HideElements) {
            iconTransform = Math::Mat4f::RotateZ(iconTransform, 90);
        }
        iconTransform = Math::Mat4f::Translate(iconTransform,
                                               assignedPos + Math::Vec2f(s_IconWidthHeight / 2.0f,
                                                                         -s_IconWidthHeight / 2.0f));
        Math::Vec2f cutoffPos = {assignedPos.GetX() + assignedMaxSize.GetX(),
                                 assignedPos.GetY() - assignedMaxSize.GetY()};
        GuiElement::s_PrimitiveShader->Bind();
        GuiElement::s_PrimitiveShader->SetUniformVec2f(cutoffPos, "u_CutoffPos");
        Renderer2D::Submit(GuiElement::s_PrimitiveShader, GuiElement::s_TriangleMesh, iconTransform);

        if(m_HideElements) { return; }
        GuiContainer::Render(assignedPos, assignedMaxSize,
                             {GuiMetrics::WindowElementIndent, -GuiMetrics::WindowElementHeight - GuiMetrics::WindowElementVerticalMargin });
    }

    bool GuiDropdown::IsHovering(const Math::Vec2f& mousePos) const
    {
        if (GuiTextButton::IsHovering(mousePos)) {
            return true;
        } else {
            for(unsigned int i = 0; i < m_Elements.size(); i++) {
                if(m_Elements[i]->IsHovering(mousePos)) {
                    return true;
                }
            }
        }
        return false;
    }

    float GuiDropdown::GetWidth() const
    {
        return GuiTextButton::GetWidth();
    }

    unsigned int GuiDropdown::GetGridDepth() const
    {
        return m_HideElements ? 1 : m_GridDepth;
    }

    std::shared_ptr<GuiElement> GuiDropdown::GetHoverElement(const Math::Vec2f& mousePos) const
    {
        return GetHoverElementInternal(mousePos, {});
    }

    // -------- WINDOW --------

    GuiWindow::GuiWindow(const std::string& title, GuiWindowID id, const Math::Vec2f& position, const Math::Vec2f& size)
            : m_Pos(position), m_Size(size), m_ID(id),
              m_Title(false, title, 14),
              m_MinimizeButton(false, "", 14, [this](){
                  m_HideElements = !m_HideElements;
              })
    {}

    std::shared_ptr<GuiElement> GuiWindow::GetHoverElement(const Math::Vec2f& mousePos) const
    {
        return GetHoverElementInternal(mousePos, GetPos());
    }

    void GuiWindow::Render(bool selected)
    {
        // Render window
        s_WindowShader->Bind();
        s_WindowShader->SetUniform1i(selected, "u_Selected");
        auto transform = Math::Mat4f::Translate(Math::Mat4f::Identity(), m_Pos + Math::Vec2f(m_Size.GetX() / 2.0f, -m_Size.GetY() / 2.0f));
        transform = Math::Mat4f::Scale(transform, m_Size);
        Renderer2D::Submit(s_WindowShader, GuiElement::s_RectMesh, transform);

        // Render title
        m_Title.Render(m_Pos + s_TitlePos, m_Size);

        // Render minimize button
        Math::Mat4f iconTransform = Math::Mat4f::Scale(Math::Mat4f::Identity(),
                                                       {s_IconWidthHeight * 0.5f, s_IconWidthHeight * 0.5f, 1.0f});
        if(m_HideElements) {
            iconTransform = Math::Mat4f::RotateZ(iconTransform, 90);
        }
        iconTransform = Math::Mat4f::Translate(iconTransform, m_Pos + s_MinimizeIconPos +
                        Math::Vec2f(s_IconWidthHeight / 2.0f, -s_IconWidthHeight / 2.0f));
        Math::Vec2f cutoffPos = GuiMetrics::GetCutoffPos(m_Pos, m_Size);
        GuiElement::s_PrimitiveShader->Bind();
        GuiElement::s_PrimitiveShader->SetUniformVec2f(cutoffPos, "u_CutoffPos");
        Renderer2D::Submit(GuiElement::s_PrimitiveShader, GuiElement::s_TriangleMesh, iconTransform);


        GuiContainer::Render(m_Pos, m_Size,
                             {GuiMetrics::WindowElementIndent,
                              -(GuiMetrics::WindowHeaderSize + GuiMetrics::WindowElementVerticalMargin) });
    }

    bool GuiWindow::IsHoveringHeader(const Math::Vec2f& mousePos)
    {
        return Math::Algo::IsPointInsideRectangle({m_Pos.GetX() + s_TitlePos.GetX(), m_Pos.GetY()},
                                                  {m_Pos.GetX() + m_Size.GetX(), m_Pos.GetY()},
                                                  {m_Pos.GetX() + m_Size.GetX(), m_Pos.GetY() - GuiMetrics::WindowHeaderSize},
                                                  {m_Pos.GetX() + s_TitlePos.GetX(), m_Pos.GetY() - GuiMetrics::WindowHeaderSize},
                                                  mousePos);
    }

    bool GuiWindow::IsHoveringResize(const Math::Vec2f& mousePos)
    {
        return Math::Algo::IsPointInsideCircle({m_Pos.GetX() + m_Size.GetX(), m_Pos.GetY() - m_Size.GetY()},
                                               10.0f, mousePos);
    }

    bool GuiWindow::IsHoveringWindow(const Math::Vec2f& mousePos)
    {
        return Math::Algo::IsPointInsideRectangle({m_Pos.GetX(), m_Pos.GetY()},
                                                  {m_Pos.GetX() + m_Size.GetX(), m_Pos.GetY()},
                                                  {m_Pos.GetX() + m_Size.GetX(), m_Pos.GetY() - m_Size.GetY()},
                                                  {m_Pos.GetX(), m_Pos.GetY() - m_Size.GetY()},
                                                  mousePos);
    }

    bool GuiWindow::IsHoveringMinimize(const Math::Vec2f& mousePos) {
        return Math::Algo::IsPointInsideRectangle({m_Pos.GetX(), m_Pos.GetY()},
                                                  {m_Pos.GetX() + s_IconWidthHeight, m_Pos.GetY()},
                                                  {m_Pos.GetX() + s_IconWidthHeight, m_Pos.GetY() - GuiMetrics::WindowHeaderSize},
                                                  {m_Pos.GetX(), m_Pos.GetY() - GuiMetrics::WindowHeaderSize},
                                                  mousePos);
    }

    void GuiWindow::Move(const Math::Vec2f& delta, const Math::Vec2f& minPos, const Math::Vec2f& maxPos)
    {
        if(!(m_Pos.GetY() + delta.GetY() > maxPos.GetY() ||
             m_Pos.GetY() - GuiMetrics::WindowHeaderSize + delta.GetY() < minPos.GetY())) {
            m_Pos = {m_Pos.GetX(), m_Pos.GetY() + delta.GetY()};
        }
        if(!(m_Pos.GetX() + delta.GetX() < minPos.GetX() ||
             m_Pos.GetX() + m_Size.GetX() + delta.GetX() > maxPos.GetX())) {
            m_Pos = {m_Pos.GetX() + delta.GetX(), m_Pos.GetY()};
        }
    }

    void GuiWindow::Resize(const Math::Vec2f& delta, const Math::Vec2f& minSize, const Math::Vec2f& maxSize)
    {
        Math::Vec2f newSize = m_Size + delta;
        newSize.Clamp(minSize, maxSize);
        m_Size = newSize;
    }

    Math::Vec2f GuiWindow::GetPos() const
    {
        return m_Pos;
    }

    GuiWindowID GuiWindow::GetID() const
    {
        return m_ID;
    }
}