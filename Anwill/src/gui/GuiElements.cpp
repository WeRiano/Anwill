#include "gui/GuiElements.h"
#include "math/Algo.h"
#include "core/Log.h"

namespace Anwill {

    Mesh GuiElement::s_RectMesh;
    std::unique_ptr<Font> GuiElement::s_Font;
    std::shared_ptr<Shader> GuiText::s_Shader;
    std::shared_ptr<Shader> GuiButton::s_Shader;
    std::shared_ptr<Shader> GuiWindow::s_WindowShader;

    // ---------- ELEMENT ----------

    GuiElement::GuiElement()
        : m_IsHovered(false), m_IsPressed(false)
    {}

    void GuiElement::Move(const Math::Vec2f& delta)
    {
        m_Pos += delta;
    }

    bool GuiElement::IsHovered() const
    {
        return m_IsHovered;
    }

    Math::Vec2f GuiElement::GetPos() const
    {
        return m_Pos;
    }

    Math::Vec2f GuiElement::GetSize() const
    {
        return m_Size;
    }

    void GuiElement::StartHovering()
    {
        m_IsHovered = true;
    }

    void GuiElement::StopHovering()
    {
        m_IsHovered = false;
    }

    void GuiElement::StartPressing()
    {
        m_IsPressed = true;
    }

    void GuiElement::StopPressing()
    {
        m_IsPressed = false;
    }

    void GuiElement::Release()
    {
        m_IsPressed = false;
    }

    // ---------- TEXT ----------

    GuiText::GuiText(const std::string& text, unsigned int textSize)
        : GuiElement(), m_Text(text), m_TextScale(Font::GetScaleValue(textSize)), m_TextWidth((float) GuiText::s_Font->GetStringWidth(text))
    {
        m_Size = { m_TextWidth * m_TextScale, GuiMetrics::WindowElementHeight };
        m_Pos += { 0.0f, -GuiMetrics::TextBaselineOffset };
    }

    void GuiText::Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize)
    {
        Math::Vec2f cutoffPos = {assignedPos.GetX() + assignedMaxSize.GetX(), assignedPos.GetY() - assignedMaxSize.GetY()};
        auto thisTransform = Math::Mat4f::Scale(Math::Mat4f::Identity(), {m_TextScale, m_TextScale, 1.0f});
        thisTransform = Math::Mat4f::Translate(thisTransform, assignedPos + m_Pos);

        // Render
        GuiText::s_Shader->Bind();
        GuiText::s_Shader->SetUniformVec2f(cutoffPos, "u_CutoffPos");
        Renderer2D::Submit(GuiText::s_Shader, *GuiElement::s_Font, m_Text, thisTransform);
    }

    bool GuiText::IsHovering(const Math::Vec2f& mousePos) { return false; }

    void GuiText::SetText(const std::string& text) {
        m_Text = text;
        m_TextWidth = (float) GuiText::s_Font->GetStringWidth(text);
    }

    // ---------- BUTTON ----------

    GuiButton::GuiButton(const std::string& text, unsigned int textSize, const std::function<void()>& callback)
        : GuiElement(), m_ButtonText(text, textSize), m_Callback(callback)
    {
        m_Size = { m_ButtonText.GetSize().GetX() + GuiMetrics::ButtonTextMargin * 2.0f, GuiMetrics::WindowElementHeight };
        m_Pos += { 0.0f, 0.0f };
        m_ButtonText.Move({ GuiMetrics::ButtonTextMargin, 0.0f});
    }

    void GuiButton::Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize)
    {
        Math::Vec2f cutoffPos = {assignedPos.GetX() + assignedMaxSize.GetX(), assignedPos.GetY() - assignedMaxSize.GetY()};
        auto thisTransform = Math::Mat4f::Scale(Math::Mat4f::Identity(), m_Size);
        thisTransform = Math::Mat4f::Translate(thisTransform, assignedPos + m_Pos + Math::Vec2f(m_Size.GetX() / 2.0f, -m_Size.GetY() / 2.0f));

        // Render button
        GuiButton::s_Shader->Bind();
        GuiButton::s_Shader->SetUniform1i(m_IsHovered, "u_Hovering");
        GuiButton::s_Shader->SetUniform1i(m_IsPressed, "u_Pressing");
        GuiButton::s_Shader->SetUniformVec2f(cutoffPos, "u_CutoffPos");
        Renderer2D::Submit(GuiButton::s_Shader, GuiElement::s_RectMesh, thisTransform);

        m_ButtonText.Render(assignedPos, assignedMaxSize);
    }

    bool GuiButton::IsHovering(const Math::Vec2f& mousePos)
    {
        return Math::Algo::IsPointInsideRectangle({m_Pos.GetX(), m_Pos.GetY()},
                                                  {m_Pos.GetX() + m_Size.GetX(), m_Pos.GetY()},
                                                  {m_Pos.GetX() + m_Size.GetX(), m_Pos.GetY() - m_Size.GetY()},
                                                  {m_Pos.GetX(), m_Pos.GetY() - m_Size.GetY()},
                                                  mousePos);
    }

    void GuiButton::Release()
    {
        GuiElement::Release();
        m_Callback();
    }

    void GuiButton::SetCallback(const std::function<void()>& callback)
    {
        m_Callback = callback;
    }

    void GuiButton::SetText(const std::string& text)
    {
        m_ButtonText.SetText(text);
    }

    // -------- DROPDOWN --------

    GuiDropdown::GuiDropdown(const std::string& text, unsigned int textSize)
        : GuiElement(), m_DropdownText(text, textSize), m_Open(false)
    {
        // -1 represents an infinite width (for now)
        m_Size = {-1.0f, GuiMetrics::WindowElementHeight};
        m_Pos = {0.0f, 0.0f};
    }

    void GuiDropdown::Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize)
    {

    }

    void GuiDropdown::Release()
    {
        GuiElement::Release();
        m_Open = !m_Open;
    }

    // -------- WINDOW --------

    GuiWindow::GuiWindow(const std::string& title, GuiWindowID id, const Math::Vec2f& position, const Math::Vec2f& size)
            : m_Pos(position), m_Size(size), m_ID(id),
              m_Title(title, 13),
              m_NextHorizontalElementPos(GuiMetrics::WindowElementMargin, -(GuiMetrics::WindowHeaderSize + GuiMetrics::WindowElementMargin)),
              m_NextVerticalElementPos(m_NextHorizontalElementPos)
    {
        // Title is a very specific type of element that doesn't have a grid location, so its grid location becomes the window origin.
        m_Title.Move({GuiMetrics::WindowBorderSize * 2 + GuiMetrics::WindowElementMargin,
                      GuiMetrics::TextBaselineOffset - GuiMetrics::WindowHeaderSize * 0.75f});
    }

    void GuiWindow::Render(bool selected)
    {
        s_WindowShader->Bind();
        s_WindowShader->SetUniform1i(selected, "u_Selected");
        s_WindowShader->Unbind();

        auto transform = Math::Mat4f::Translate(Math::Mat4f::Identity(), m_Pos + Math::Vec2f(m_Size.GetX() / 2.0f, -m_Size.GetY() / 2.0f));
        transform = Math::Mat4f::Scale(transform, m_Size);
        Renderer2D::Submit(s_WindowShader, GuiElement::s_RectMesh, transform);

        m_Title.Render(m_Pos, m_Size);

        for (unsigned int i = 0; i < m_Elements.size(); i++) {
            m_Elements[i].first->Render(m_Pos + m_Elements[i].second, { m_Size.GetX() - m_Elements[i].second.GetX() - GuiMetrics::WindowCutoffMargin,
                                                                        m_Size.GetY() + m_Elements[i].second.GetY() - GuiMetrics::WindowCutoffMargin });
        }
    }

    bool GuiWindow::IsHoveringHeader(const Math::Vec2f& mousePos)
    {
        return Math::Algo::IsPointInsideRectangle({m_Pos.GetX(), m_Pos.GetY()},
                                                  {m_Pos.GetX() + m_Size.GetX(), m_Pos.GetY()},
                                                  {m_Pos.GetX() + m_Size.GetX(), m_Pos.GetY() - GuiMetrics::WindowHeaderSize},
                                                  {m_Pos.GetX(), m_Pos.GetY() - GuiMetrics::WindowHeaderSize},
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

    std::shared_ptr<GuiElement> GuiWindow::GetHoverElement(const Math::Vec2f& mousePos)
    {
        // TODO: Can (probably) optimize!
        for(unsigned int i = 0; i < m_Elements.size(); i++) {
            if(m_Elements[i].first->IsHovering( mousePos - (GetPos() + m_Elements[i].second)) ) {
                return m_Elements[i].first;
            }
        }
        return nullptr;
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

    unsigned int GuiWindow::GetElementCount() const
    {
        return m_Elements.size();
    }
}