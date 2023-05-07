#include "gui/GuiElements.h"
#include "math/Algo.h"
#include "core/Log.h"
#include "utils/Utils.h"

namespace Anwill {

    Math::Vec2f GuiMetrics::windowSize;
    Mesh GuiElement::s_RectMesh;
    Mesh GuiElement::s_TriangleMesh;
    std::shared_ptr<Shader> GuiElement::s_PrimitiveShader;
    std::unique_ptr<Font> GuiElement::s_Font;
    std::shared_ptr<Shader> GuiText::s_Shader;
    std::shared_ptr<Shader> GuiTextButton::s_Shader;
    Mesh GuiCheckbox::s_CheckmarkMesh;
    const Math::Vec2f GuiSlider::s_MarkerSize = {13.0f,
                                                 GuiMetrics::WindowElementHeight - 1.5f * 2.0f};

    // ---------- ICON ----------

    void GuiIcon::RenderRightArrow(const Math::Vec2f& assignedPos,
                                   const Math::Vec2f& assignedSize,
                                   const Math::Vec2f& assignedMaxSize,
                                   const Math::Vec3f& color) {
        Math::Mat4f iconTransform = Math::Mat4f::Scale(Math::Mat4f::Identity(),
                                                       {assignedSize.GetX(), assignedSize.GetY(), 1.0f});
        iconTransform = Math::Mat4f::RotateZ(iconTransform, 90);
        iconTransform = Math::Mat4f::Translate(iconTransform, assignedPos
        + Math::Vec2f(assignedSize.GetX(), -assignedSize.GetY()));

        Math::Vec2f cutoffPos = GuiMetrics::GetCutoffPos(assignedPos, assignedMaxSize);
        GuiElement::s_PrimitiveShader->Bind();
        GuiElement::s_PrimitiveShader->SetUniformVec2f(cutoffPos, "u_CutoffPos");
        GuiElement::s_PrimitiveShader->SetUniformVec3f(color, "u_Color");
        Renderer2D::Submit(GuiElement::s_PrimitiveShader, GuiElement::s_TriangleMesh, iconTransform);
    }

    void GuiIcon::RenderDownArrow(const Math::Vec2f& assignedPos,
                                  const Math::Vec2f& assignedSize,
                                  const Math::Vec2f& assignedMaxSize,
                                  const Math::Vec3f& color) {
        Math::Mat4f iconTransform = Math::Mat4f::Scale(Math::Mat4f::Identity(),
                                                       {assignedSize.GetX(), assignedSize.GetY(), 1.0f});
        iconTransform = Math::Mat4f::Translate(iconTransform, assignedPos
        + Math::Vec2f(assignedSize.GetX(), -assignedSize.GetY()));

        Math::Vec2f cutoffPos = GuiMetrics::GetCutoffPos(assignedPos, assignedMaxSize);
        GuiElement::s_PrimitiveShader->Bind();
        GuiElement::s_PrimitiveShader->SetUniformVec2f(cutoffPos, "u_CutoffPos");
        GuiElement::s_PrimitiveShader->SetUniformVec3f(color, "u_Color");
        Renderer2D::Submit(GuiElement::s_PrimitiveShader, GuiElement::s_TriangleMesh, iconTransform);
    }

    void GuiIcon::RenderCross(const Math::Vec2f& assignedPos,
                              const Math::Vec2f& assignedSize,
                              const Math::Vec2f& assignedMaxSize,
                              const Math::Vec3f& color) {
        // TODO
    }

    void GuiIcon::RenderCheckmark(const Math::Vec2f &assignedPos,
                                  const Math::Vec2f &assignedSize,
                                  const Math::Vec2f &assignedMaxSize,
                                  const Math::Vec3f& color) {
        Math::Mat4f iconTransform = Math::Mat4f::Scale(Math::Mat4f::Identity(),
                                                       {assignedSize.GetX(), assignedSize.GetY(), 1.0f});
        iconTransform = Math::Mat4f::Translate(iconTransform, assignedPos
        + (Math::Vec2f(assignedSize.GetX(), -assignedSize.GetY()) * 0.5f));

        Math::Vec2f cutoffPos = GuiMetrics::GetCutoffPos(assignedPos, assignedMaxSize);
        GuiElement::s_PrimitiveShader->Bind();
        GuiElement::s_PrimitiveShader->SetUniformVec2f(cutoffPos, "u_CutoffPos");
        GuiElement::s_PrimitiveShader->SetUniformVec3f(color, "u_Color");
        Renderer2D::Submit(GuiElement::s_PrimitiveShader, GuiCheckbox::s_CheckmarkMesh, iconTransform);
    }

    void GuiIcon::RenderRectangle(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedSize,
                                  const Math::Vec2f& assignedMaxSize, const Math::Vec3f& color)
    {
        Math::Mat4f iconTransform = Math::Mat4f::Scale(Math::Mat4f::Identity(), assignedSize);
        iconTransform = Math::Mat4f::Translate(iconTransform, assignedPos
                                                              + (Math::Vec2f(assignedSize.GetX(), -assignedSize.GetY()) * 0.5f));

        Math::Vec2f cutoffPos = GuiMetrics::GetCutoffPos(assignedPos, assignedMaxSize);
        GuiElement::s_PrimitiveShader->Bind();
        GuiElement::s_PrimitiveShader->SetUniformVec2f(cutoffPos, "u_CutoffPos");
        GuiElement::s_PrimitiveShader->SetUniformVec3f(color, "u_Color");
        Renderer2D::Submit(GuiElement::s_PrimitiveShader, GuiCheckbox::s_RectMesh, iconTransform);
    }

    // ---------- ELEMENT ----------

    GuiElement::GuiElement()
        : m_IsHovered(false), m_IsPressed(false)
    {}

    void GuiElement::StartHovering()
    {
        m_IsHovered = true;
    }

    void GuiElement::StopHovering()
    {
        m_IsHovered = false;
    }

    void GuiElement::OnHover(const Math::Vec2f& mousePos)
    {
        //AW_INFO("Hovering!");
        // Default behavior is nothing
    }

    void GuiElement::StartPressing()
    {
        //AW_INFO("Start pressing ...");
        m_IsPressed = true;
    }

    void GuiElement::OnPress(const Math::Vec2f& mousePos)
    {
        //AW_INFO("Pressing!");
        // Default behavior is nothing
    }

    void GuiElement::Release()
    {
        //AW_INFO("Stop pressing ...");
        m_IsPressed = false;
    }

    bool GuiElement::IsHovered() const
    {
        return m_IsHovered;
    }

    // ---------- TEXT ----------

    GuiText::GuiText(const std::string& text, unsigned int textSize)
        : GuiElement(),
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

    GuiButton::GuiButton(const Math::Vec2f& size, const std::function<void()>& callback)
        : GuiElement(),
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

    GuiTextButton::GuiTextButton(const std::string& text, unsigned int textSize,
                                 const std::function<void()>& callback)
        : GuiButton({}, callback),
          m_Text(text, textSize)
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

    GuiCheckbox::GuiCheckbox(bool startAsChecked,
                             const std::function<void(bool)>& callback)
        : GuiButton({GuiMetrics::WindowElementHeight, GuiMetrics::WindowElementHeight},
                    [this, callback](){
            if(m_IsHovered) {
                m_Checked = !m_Checked;
                callback(m_Checked);
            }
        }), m_Checked(startAsChecked)
    {}

    void GuiCheckbox::Render(const Math::Vec2f &assignedPos, const Math::Vec2f &assignedMaxSize) {
        GuiButton::Render(assignedPos, assignedMaxSize);

        // Render checkmark
        if(!m_Checked) { return; }
        GuiIcon::RenderCheckmark(assignedPos + Math::Vec2f(GuiMetrics::CheckboxElementMargin, -GuiMetrics::CheckboxElementMargin * 2.0f),
                                 {m_ButtonSize.GetX() - GuiMetrics::CheckboxElementMargin * 2.0f,
                                  m_ButtonSize.GetY() - GuiMetrics::CheckboxElementMargin * 4.0f},
                                 assignedMaxSize,
                                 {1.0f, 1.0f, 1.0f});
    }

    // ---------- SLIDER ----------

    GuiSlider::GuiSlider()
        : GuiButton({GuiMetrics::WindowElementHeight * 7.0f, GuiMetrics::WindowElementHeight}, [](){}),
          m_ValueText("", GuiMetrics::FontSize)
    {}

    void GuiSlider::Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize)
    {
        // Render background button
        GuiButton::Render(assignedPos, assignedMaxSize);

        // Render "slider" (marker) rectangle
        Math::Vec2f markerPos = {m_LastCursorXPos - s_MarkerSize.GetX() * 0.5f,
                                 -(m_ButtonSize.GetY() - s_MarkerSize.GetY()) * 0.5f + 1.0f};
        markerPos = {Utils::Clamp(markerPos.GetX(), 0.0f, GetWidth() - s_MarkerSize.GetX()),
                     markerPos.GetY()};
        GuiIcon::RenderRectangle(assignedPos + markerPos, s_MarkerSize,
                                 assignedMaxSize - markerPos, {0.45f, 0.45f, 1.0f});

        // Render text
        float centeredTextXPos = m_ButtonSize.GetX() * 0.5f - m_ValueText.GetWidth() * 0.5f;
        m_ValueText.Render(assignedPos + Math::Vec2f(centeredTextXPos, 0.0f),
                      assignedMaxSize - Math::Vec2f(GuiMetrics::ButtonTextMargin, 0.0f));
    }

    void GuiSlider::OnPress(const Math::Vec2f &mousePos) {
        GuiElement::OnPress(mousePos);

        m_LastCursorXPos = mousePos.GetX();
    }

    // ---------- INT SLIDER ----------

    GuiIntSlider::GuiIntSlider(int min, int max, int* sliderValue)
        : m_Min(min), m_Max(max), m_ClientValuePointer(sliderValue)
    {
        int startValue = m_Min;
        m_ValueText.SetText(std::to_string(startValue));
        m_LastCursorXPos = 0.0f;
    }

    void GuiIntSlider::OnPress(const Math::Vec2f &mousePos) {
        GuiSlider::OnPress(mousePos);

        m_LastCursorXPos = mousePos.GetX();
        float sliderValue = Utils::ScaleToRange(mousePos.GetX(),
                                                static_cast<float>(m_Min),
                                                static_cast<float>(m_Max),
                                                s_MarkerSize.GetX() * 0.5f,
                                                GetWidth() - s_MarkerSize.GetX() * 0.5f);
        sliderValue = Utils::Clamp(sliderValue,
                                   static_cast<float>(m_Min),
                                   static_cast<float>(m_Max));
        int roundedInt = Utils::RoundToInt(sliderValue);
        *m_ClientValuePointer = roundedInt;
        m_ValueText.SetText(std::to_string(roundedInt));
    }

    // ---------- FLOAT SLIDER ----------

    GuiFloatSlider::GuiFloatSlider(float min, float max, float* sliderValue)
        : m_Min(min), m_Max(max), m_ClientValuePointer(sliderValue)
    {
        float startValue = m_Min;
        m_ValueText.SetText(Utils::RoundFloatToString(startValue, 3));
        m_LastCursorXPos = 0.0f;
    }

    void GuiFloatSlider::OnPress(const Math::Vec2f &mousePos) {
        GuiSlider::OnPress(mousePos);

        float sliderValue = Utils::ScaleToRange(mousePos.GetX(),
                                                static_cast<float>(m_Min),
                                                static_cast<float>(m_Max),
                                                s_MarkerSize.GetX() * 0.5f,
                                                GetWidth() - s_MarkerSize.GetX() * 0.5f);
        sliderValue = Utils::Clamp(sliderValue,
                                   static_cast<float>(m_Min),
                                   static_cast<float>(m_Max));
        *m_ClientValuePointer = sliderValue;
        m_ValueText.SetText(Utils::RoundFloatToString(sliderValue, 3));
    }
}