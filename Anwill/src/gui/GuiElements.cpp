#include "gui/GuiElements.h"
#include "math/Algo.h"
#include "core/Log.h"

namespace Anwill {

    Math::Vec2f GuiMetrics::windowSize;
    Mesh GuiElement::s_RectMesh;
    Mesh GuiElement::s_TriangleMesh;
    std::shared_ptr<Shader> GuiElement::s_PrimitiveShader;
    std::unique_ptr<Font> GuiElement::s_Font;
    std::shared_ptr<Shader> GuiText::s_Shader;
    std::shared_ptr<Shader> GuiTextButton::s_Shader;
    Mesh GuiCheckbox::s_CheckmarkMesh;

    // ---------- ICON ----------

    void GuiIcon::RenderRightArrow(const Math::Vec2f& assignedPos,
                                   const Math::Vec2f& assignedSize,
                                   const Math::Vec2f& assignedMaxSize) {
        Math::Mat4f iconTransform = Math::Mat4f::Scale(Math::Mat4f::Identity(),
                                                       {assignedSize.GetX(), assignedSize.GetY(), 1.0f});
        iconTransform = Math::Mat4f::RotateZ(iconTransform, 90);
        iconTransform = Math::Mat4f::Translate(iconTransform, assignedPos
        + Math::Vec2f(assignedSize.GetX(), -assignedSize.GetY()));

        Math::Vec2f cutoffPos = GuiMetrics::GetCutoffPos(assignedPos, assignedMaxSize);
        GuiElement::s_PrimitiveShader->Bind();
        GuiElement::s_PrimitiveShader->SetUniformVec2f(cutoffPos, "u_CutoffPos");
        Renderer2D::Submit(GuiElement::s_PrimitiveShader, GuiElement::s_TriangleMesh, iconTransform);
    }

    void GuiIcon::RenderDownArrow(const Math::Vec2f& assignedPos,
                                  const Math::Vec2f& assignedSize,
                                  const Math::Vec2f& assignedMaxSize) {
        Math::Mat4f iconTransform = Math::Mat4f::Scale(Math::Mat4f::Identity(),
                                                       {assignedSize.GetX(), assignedSize.GetY(), 1.0f});
        iconTransform = Math::Mat4f::Translate(iconTransform, assignedPos
        + Math::Vec2f(assignedSize.GetX(), -assignedSize.GetY()));

        Math::Vec2f cutoffPos = GuiMetrics::GetCutoffPos(assignedPos, assignedMaxSize);
        GuiElement::s_PrimitiveShader->Bind();
        GuiElement::s_PrimitiveShader->SetUniformVec2f(cutoffPos, "u_CutoffPos");
        Renderer2D::Submit(GuiElement::s_PrimitiveShader, GuiElement::s_TriangleMesh, iconTransform);
    }

    void GuiIcon::RenderCross(const Math::Vec2f& assignedPos,
                              const Math::Vec2f& assignedSize,
                              const Math::Vec2f& assignedMaxSize) {
        // TODO
    }

    void GuiIcon::RenderCheckmark(const Math::Vec2f &assignedPos,
                                 const Math::Vec2f &assignedSize,
                                 const Math::Vec2f &assignedMaxSize) {
        Math::Mat4f iconTransform = Math::Mat4f::Scale(Math::Mat4f::Identity(),
                                                       {assignedSize.GetX(), assignedSize.GetY(), 1.0f});
        iconTransform = Math::Mat4f::Translate(iconTransform, assignedPos
        + (Math::Vec2f(assignedSize.GetX(), -assignedSize.GetY()) * 0.5f));

        Math::Vec2f cutoffPos = GuiMetrics::GetCutoffPos(assignedPos, assignedMaxSize);
        GuiElement::s_PrimitiveShader->Bind();
        GuiElement::s_PrimitiveShader->SetUniformVec2f(cutoffPos, "u_CutoffPos");
        Renderer2D::Submit(GuiElement::s_PrimitiveShader, GuiCheckbox::s_CheckmarkMesh, iconTransform);
    }

    // ---------- ELEMENT ----------

    GuiElement::GuiElement(bool onNewRow, bool forceNextToNewRow)
        : m_IsHovered(false), m_IsPressed(false),
          m_OnNewRow(onNewRow), m_ForceNextToNewRow(forceNextToNewRow)
    {}

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

    bool GuiElement::IsHovered() const
    {
        return m_IsHovered;
    }

    bool GuiElement::OnNewRow() const
    {
        return m_OnNewRow;
    }

    bool GuiElement::ForceNextToNewRow() const
    {
        return m_ForceNextToNewRow;
    }

    // ---------- TEXT ----------

    GuiText::GuiText(bool onNewRow, const std::string& text, unsigned int textSize)
        : GuiElement(onNewRow, false),
          m_TextPos(1.0f, -GuiMetrics::WindowElementHeight / 2.0f + GuiMetrics::TextBaselineOffset),
          m_Text(text),
          m_TextScale(Font::GetScaleValue(textSize)), m_TextWidth((float) GuiText::s_Font->GetStringWidth(text) * m_TextScale)
    {}

    void GuiText::Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize)
    {
        Math::Vec2f cutoffPos = GuiMetrics::GetCutoffPos(Math::Vec2f(m_TextPos.GetX(), 0.0f) + assignedPos, assignedMaxSize);
        auto thisTransform = Math::Mat4f::Scale(Math::Mat4f::Identity(),
                                                {m_TextScale, m_TextScale, 1.0f});
        thisTransform = Math::Mat4f::Translate(thisTransform, m_TextPos + assignedPos);

        // Render
        GuiText::s_Shader->Bind();
        GuiText::s_Shader->SetUniformVec2f(cutoffPos, "u_CutoffPos");
        Renderer2D::Submit(GuiText::s_Shader, *GuiElement::s_Font, m_Text, thisTransform);
    }

    bool GuiText::IsHovering(const Math::Vec2f& mousePos) const {
        return false;
    }

    float GuiText::GetWidth() const
    {
        return m_TextWidth;
    }

    unsigned int GuiText::GetGridDepth() const
    {
        return 1;
    }

    void GuiText::SetText(const std::string& text) {
        m_Text = text;
        m_TextWidth = (float) GuiText::s_Font->GetStringWidth(text) * m_TextScale;
    }

    // ---------- BUTTON ----------

    GuiButton::GuiButton(bool onNewRow, const Math::Vec2f& size, const std::function<void()>& callback)
        : GuiElement(onNewRow, false),
          m_ButtonSize(size),
          m_Callback(callback)
    {}

    void GuiButton::Render(const Math::Vec2f &assignedPos, const Math::Vec2f &assignedMaxSize) {
        Math::Vec2f cutoffPos = GuiMetrics::GetCutoffPos(assignedPos, assignedMaxSize);
        auto thisTransform = Math::Mat4f::Scale(Math::Mat4f::Identity(), m_ButtonSize);
        thisTransform = Math::Mat4f::Translate(thisTransform,
                                               assignedPos + Math::Vec2f(m_ButtonSize.GetX() / 2.0f, -m_ButtonSize.GetY() / 2.0f));

        // Render button
        GuiTextButton::s_Shader->Bind();
        GuiTextButton::s_Shader->SetUniform1i(m_IsHovered, "u_Hovering");
        GuiTextButton::s_Shader->SetUniform1i(m_IsPressed, "u_Pressing");
        GuiTextButton::s_Shader->SetUniformVec2f(cutoffPos, "u_CutoffPos");
        Renderer2D::Submit(GuiTextButton::s_Shader, GuiElement::s_RectMesh, thisTransform);
    }

    bool GuiButton::IsHovering(const Math::Vec2f& mousePos) const
    {
        return Math::Algo::IsPointInsideRectangle({0.0f,                 0.0f},
                                                  {m_ButtonSize.GetX(),  0.0f},
                                                  {m_ButtonSize.GetX(), -m_ButtonSize.GetY()},
                                                  {0.0f,                -m_ButtonSize.GetY()},
                                                  mousePos);
    }

    float GuiButton::GetWidth() const
    {
        return m_ButtonSize.GetX();
    }

    unsigned int GuiButton::GetGridDepth() const
    {
        return 1;
    }

    void GuiButton::Release()
    {
        GuiElement::Release();
        m_Callback();
    }

    void GuiButton::SetCallback(const std::function<void()>& callback) {
        m_Callback = callback;
    }

    // ---------- TEXT BUTTON ----------

    GuiTextButton::GuiTextButton(bool onNewRow, const std::string& text, unsigned int textSize,
                                 const std::function<void()>& callback)
        : GuiButton(onNewRow, {}, callback),
          m_Text(false, text, textSize)
    {
        m_ButtonSize = {m_Text.GetWidth() + GuiMetrics::ButtonTextMargin * 2.0f, GuiMetrics::WindowElementHeight};
    }

    void GuiTextButton::Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize)
    {
        GuiButton::Render(assignedPos, assignedMaxSize);

        // Render text
        m_Text.Render(assignedPos + Math::Vec2f(GuiMetrics::ButtonTextMargin, 0.0f),
                      assignedMaxSize - Math::Vec2f(GuiMetrics::ButtonTextMargin, 0.0f));
    }

    void GuiTextButton::SetText(const std::string& text) {
        m_Text.SetText(text);
        m_ButtonSize = { m_Text.GetWidth() + GuiMetrics::ButtonTextMargin * 2.0f, m_ButtonSize.GetY() };
    }

    // ---------- CHECKBOX ----------

    GuiCheckbox::GuiCheckbox(bool onNewRow,
                             bool startAsChecked,
                             const std::function<void(bool)>& callback)
        : GuiButton(onNewRow, {GuiMetrics::WindowElementHeight, GuiMetrics::WindowElementHeight},
                    [this, callback](){
            m_Checked = !m_Checked;
            callback(m_Checked);
        }), m_Checked(startAsChecked)
    {}

    void GuiCheckbox::Render(const Math::Vec2f &assignedPos, const Math::Vec2f &assignedMaxSize) {
        GuiButton::Render(assignedPos, assignedMaxSize);

        // Render checkmark
        if(!m_Checked) { return; }
        GuiIcon::RenderCheckmark(assignedPos + Math::Vec2f(GuiMetrics::CheckboxElementMargin, -GuiMetrics::CheckboxElementMargin * 2.0f),
                                 {m_ButtonSize.GetX() - GuiMetrics::CheckboxElementMargin * 2.0f,
                                  m_ButtonSize.GetY() - GuiMetrics::CheckboxElementMargin * 4.0f},
                                 assignedMaxSize);
    }
}