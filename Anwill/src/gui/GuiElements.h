 #pragma once

#include <vector>
#include <functional>
#include <iostream>
#include <stack>

#include "gui/GuiStyling.h"
#include "gfx/Mesh.h"
#include "gfx/Font.h"
#include "gfx/Renderer2D.h"
#include "math/Math.h"
#include "math/Mat4f.h"
#include "utils/Utils.h"

namespace Anwill {

    class GuiIcon {
    public:
        static void RenderRightArrow(const Math::Vec2f& assignedPos,
                                     const Math::Vec2f& assignedSize,
                                     const Math::Vec2f& assignedMaxSize,
                                     const Math::Vec3f& color);
        static void RenderDownArrow(const Math::Vec2f& assignedPos,
                                    const Math::Vec2f& assignedSize,
                                    const Math::Vec2f& assignedMaxSize,
                                    const Math::Vec3f& color);
        static void RenderCross(const Math::Vec2f& assignedPos,
                                const Math::Vec2f& assignedSize,
                                const Math::Vec2f& assignedMaxSize,
                                const Math::Vec3f& color);
        static void RenderCheckmark(const Math::Vec2f& assignedPos,
                                    const Math::Vec2f& assignedSize,
                                    const Math::Vec2f& assignedMaxSize,
                                    const Math::Vec3f& color);
        static void RenderRectangle(const Math::Vec2f& assignedPos,
                                    const Math::Vec2f& assignedSize,
                                    const Math::Vec2f& assignedMaxSize,
                                    const Math::Vec3f& color);
        static void RenderEllipse(const Math::Vec2f& assignedPos,
                                 const Math::Vec2f& assignedSize,
                                 const Math::Vec2f& assignedMaxSize,
                                 const Math::Vec3f& color);
    };

    class GuiTooltip {
    public:
        GuiTooltip(const std::string& tooltipText, unsigned int tooltipTextSize);

        void Render(const Math::Vec2f& mousePos, const Math::Vec2f& gameWindowSize);
    private:
        std::string m_TooltipText;
        float m_TooltipTextScale;
    };

    class GuiElement {
    public:
        GuiElement();

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
        virtual void Release(); // StopPressing()

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
    };

    class GuiText : public GuiElement {
    public:
        GuiText(const std::string& text, unsigned int textSize);

        void Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                    const Timestamp& delta) override;
        void Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                    const Timestamp& delta, int startIndex, int endIndex);
        bool IsHovering(const Math::Vec2f& mousePos) const override;
        float GetWidth() const override;
        float GetWidth(unsigned char c) const;
        float GetWidth(unsigned int startIndex, unsigned int size) const;
        unsigned int GetGridDepth() const override;

        void Set(const std::string& text);
        void AddCharacter(unsigned char c, unsigned int index);
        void PrependCharacter(unsigned char c);
        void AppendCharacter(unsigned char c);
        char RemoveCharacter(unsigned int characterIndex);
        std::string RemoveCharacters(unsigned int startCharacterIndex, unsigned int endCharacterIndex);
        /**
         * @brief Remove the rightmost char from the string.
         */
        void TruncateCharacter();
        /**
         * @brief Remove the leftmost char from the string.
         * @return The popped char.
         */
        unsigned char PopCharacter();
        std::string ToString() const;

    protected:
        Math::Vec2f m_TextPos;
        std::string m_Text;
        float m_TextScale, m_TextWidth; // Calculate TextScale each iteration? Cheap.
    };

    class GuiButton : public GuiElement {
    public:
        GuiStyling::Button m_ButtonStyle;

        GuiButton(const Math::Vec2f& size,
                  const std::function<void()>& callback);

        void Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                    const Timestamp& delta) override;
        bool IsHovering(const Math::Vec2f& mousePos) const override;
        float GetWidth() const override;
        unsigned int GetGridDepth() const override;
        void Release() override;

        void SetCallback(const std::function<void()>& callback);
        void SetWidth(float width);
        void SetHeight(float height);
    protected:
        Math::Vec2f m_ButtonSize;
        std::function<void()> m_Callback;
    };

    class GuiTextButton : public GuiButton {
    public:
        GuiTextButton(const std::string& text,
                      unsigned int textSize,
                      const std::function<void()>& callback);

        void Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                    const Timestamp& delta) override;
        void SetText(const std::string& text);

    protected:
        GuiText m_Text;
    };

    class GuiCheckbox : public GuiButton  {
    public:
        GuiStyling::Checkbox m_CheckboxStyle;

        GuiCheckbox(bool checked, const std::string& text, unsigned int textSize,
                    const std::function<void(bool)>& callback);

        void Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                    const Timestamp& delta) override;
        float GetWidth() const override;

    protected:
        GuiText m_Text;
        bool m_Checked;
    };

    class GuiRadioButton : public GuiButton {
    public:
        GuiStyling::RadioButton m_RadioButtonStyle;

        GuiRadioButton(const std::string& text, unsigned int textSize, int& reference,
                       const int onSelectValue, const std::function<void()>& callback);

        void Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                    const Timestamp& delta) override;
        float GetWidth() const override;

    protected:
        GuiText m_Text;
        int& m_Reference;
        const int m_OnSelectValue;
    };

    template <typename T>
    class GuiSlider : public GuiButton {
    public:
        GuiStyling::Slider m_SliderStyle;

        GuiSlider(T min, T max, T& source)
            : m_Min(min), m_Max(max), m_Source(source),
              GuiButton({GuiStyling::Window::elementHeight * 7.0f, GuiStyling::Window::elementHeight},
                        [](){}),
              m_ValueText("", GuiStyling::Text::fontSize)
        {
            if(std::is_same<T, float>::value)
            {
                m_OnPressSet = [](float f) { return f; };
                m_SetValueText = [this](){ m_ValueText.Set(Math::RoundToString(m_Source, 2)); };
            }
            else if(std::is_same<T, int>::value)
            {
                m_OnPressSet = [](float f){ return std::round(f); };
                m_SetValueText = [this](){ m_ValueText.Set(std::to_string(m_Source)); };
            } else {
                // TODO: Double, long, etc
                AW_FATAL("GuiSlider template type not supported");
            }
        }

        void Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                    const Timestamp& delta) override
        {
            // Render background button
            GuiButton::Render(assignedPos, assignedMaxSize, delta);

            // Render "slider" (marker) rectangle
            float markerXPosDelta = Math::ScaleToRange<float>(static_cast<float>(m_Source),
                                                               m_MarkerXOffset,
                                                               GuiButton::GetWidth() - m_MarkerXOffset,
                                                               static_cast<float>(m_Min),
                                                               static_cast<float>(m_Max));
            markerXPosDelta = Math::Clamp(markerXPosDelta, m_MarkerXOffset, GetWidth() - m_MarkerXOffset);
            Math::Vec2f markerPos = {markerXPosDelta - m_MarkerXOffset,
                                     -(m_ButtonSize.Y - GuiStyling::Slider::markerSize.Y) * 0.5f + 1.0f};

            GuiIcon::RenderRectangle(assignedPos + markerPos, GuiStyling::Slider::markerSize,
                                     assignedMaxSize - markerPos, m_SliderStyle.markerColor);

            // Render text
            m_SetValueText();
            float centeredTextXPos = m_ButtonSize.X * 0.5f - m_ValueText.GetWidth() * 0.5f;
            m_ValueText.Render(assignedPos + Math::Vec2f(centeredTextXPos, 0.0f),
                               assignedMaxSize - Math::Vec2f(centeredTextXPos + GuiStyling::TextButton::textPadding, 0.0f),
                               delta);
        }

        void OnPress(const Math::Vec2f& mousePos) override {
            float t = Math::ScaleToRange<float>(mousePos.X,
                                             static_cast<float>(m_Min),
                                             static_cast<float>(m_Max),
                                             m_MarkerXOffset,
                                             GetWidth() - m_MarkerXOffset);
            t = Math::Clamp(t, static_cast<float>(m_Min), static_cast<float>(m_Max));
            m_Source = m_OnPressSet(t);
        }

    protected:
        const float m_MarkerXOffset = GuiStyling::Slider::markerSize.X * 0.5f;

        T m_Min, m_Max;
        T& m_Source;
        GuiText m_ValueText;
        std::function<T(float)> m_OnPressSet;
        std::function<void()> m_SetValueText;
    };

    class GuiInputText : public GuiTextButton {
    public:
        GuiInputText(const std::string& startText, unsigned int textSize, float pixelWidth);

        void Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                    const Timestamp& delta) override;
        void OnKeyPress(const KeyCode& keyCode) override;
        void OnKeyRepeat(const KeyCode& keyCode) override;
        void OnKeyChar(unsigned char c) override;

    private:
        void RenderButton();
        void RenderSelected(const Math::Vec2f& assignedPos, const Math::Vec2f& offset);
        void RenderText(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                        const Timestamp& delta);
        void RenderCursor(const Math::Vec2f& assignedPos, const Math::Vec2f& offset);

        void KeycodeToAction(const KeyCode& keyCode);
        void DeleteText();
        void RefillOverflowFromLeft();
        void RefillOverflow();
        void RemoveSelectedCharacters();
        void RemoveCharacterAtCursor();
        void ResetSelect();
        void SelectAll();
        void MoveRight();
        void MoveLeft();
        bool IsTextWiderThanBox() const;
        bool IsTextWiderThanBox(int leftIndex, int rightIndex) const;
        void CalcCursorTimeInterval(const Timestamp& delta);
        void DebugIndices() const;

        /// Styling
        GuiStyling::InputText m_InputTextStyle;
        /// Indicates what part of the string to render
        /// The left index is equal to the number of characters that are hidden due to overflow
        int m_RenderLeftIndex, m_RenderRightIndex;
        /// Indicates what part of the string that is selected
        /// Can be larger than
        int m_SelectLeftIndex, m_SelectRightIndex;
        /// Counter used for 'blinking' cursor
        long double m_TimeCountMS;
        /// Indicates the string position of the cursor
        int m_CursorIndex;
        /// Indicates if the cursor should be rendered or not
        bool m_ShowCursor;
    };

    class GuiImage : public GuiElement {
    public:
        GuiImage(const std::string& fileName, unsigned int maxRows);

        void Render(const Math::Vec2f& assignedPos,
                    const Math::Vec2f& assignedMaxSize,
                    const Timestamp& delta) override;

        bool IsHovering(const Math::Vec2f& mousePos) const override;
        float GetWidth() const override;
        unsigned int GetGridDepth() const override;

    private:
        std::shared_ptr<Texture> m_Texture;
        float m_ScaleFactor;
    };

    struct ContainerElement {
    public:
        std::shared_ptr<GuiElement> element;
        Math::Vec2f position;
        bool onNewRow, forceNextToNewRow;
        bool isHidden;

        ContainerElement(std::shared_ptr<GuiElement> element, Math::Vec2f position,
                         bool onNewRow, bool forceNextToNewRow, bool isHidden)
            : element(element), position(position), onNewRow(onNewRow),
              forceNextToNewRow(forceNextToNewRow), isHidden(isHidden)
        {}
    };

    class GuiContainer {
    public:
        GuiContainer();

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
        bool IsHidingElements() const;
        void ScrollUp();
        void ScrollDown();

        template <class E, typename... Args>
        std::shared_ptr<E> AddElement(bool onNewRow, bool forceNextToNewRow, Args&&... args) {
            if(m_ContainerElements.empty() || (onNewRow || m_ContainerElements.back().forceNextToNewRow)) {
                m_GridDepth++;
            }
            m_ContainerElements.emplace_back(std::make_shared<E>(std::forward<Args>(args)...),
                    Math::Vec2f(), onNewRow, forceNextToNewRow, false);
            return std::static_pointer_cast<E>(m_ContainerElements.back().element);
        }

    protected:
        static constexpr float s_ScrollSpeed = 5.0f;
        GuiButton m_Scrollbar;
        Math::Vec2f m_ScrollOffset, m_HiddenSize;
        bool m_CanScroll;
        unsigned int m_GridDepth;
        std::vector<ContainerElement> m_ContainerElements;
        // TODO: This is volatile because?
        volatile bool m_HideElements;

        void RenderVerticalScrollbar(const Math::Vec2f& assignedPos, float visibleHeight,
                                     const Timestamp& delta);
    };

    class GuiDropdown : public GuiTextButton, public GuiContainer {
        /*
         * Inherited behavior from GuiTextButton:
         * - Dropdown toggle is a button, but with maximum allowed width and an arrow icon
         */
    public:
        GuiDropdown(const std::string& text, unsigned int textSize);

        void Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                    const Timestamp& delta) override;
        bool IsHovering(const Math::Vec2f& mousePos) const override;
        float GetWidth() const override;
        unsigned int GetGridDepth() const override;
    };

    typedef unsigned int GuiWindowID;

    class GuiWindow : public GuiContainer {
    public:
        GuiWindow(const std::string& title, GuiWindowID id,
                  const Math::Vec2f& position, const Math::Vec2f& size);

        std::shared_ptr<GuiElement> GetHoverElement(Math::Vec2f& hoverElementPos,
                                                    const Math::Vec2f& mousePos) const override;
        void Render(bool isSelected, const Timestamp& delta);
        bool IsHoveringHeader(const Math::Vec2f& mousePos);
        bool IsHoveringResize(const Math::Vec2f& mousePos);
        bool IsHoveringWindow(const Math::Vec2f& mousePos);
        bool IsHoveringMinimize(const Math::Vec2f& mousePos);
        void Move(const Math::Vec2f& delta, const Math::Vec2f& minPos, const Math::Vec2f& maxPos);
        void Resize(const Math::Vec2f& delta, const Math::Vec2f& minSize, const Math::Vec2f& maxSize);
        Math::Vec2f GetPos() const;
        GuiWindowID GetID() const;

    private:
        Math::Vec2f m_Pos, m_Size, m_LastShowSize;
        GuiWindowID m_ID;
        GuiText m_Title;
        std::shared_ptr<GuiButton> m_MinimizeButton;
    };
}