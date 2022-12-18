#pragma once

#include "gui/GuiElements.h"

namespace Anwill {

    class GuiContainer {
    public:
        GuiContainer();

        virtual std::shared_ptr<GuiElement> GetHoverElement(const Math::Vec2f& mousePos) const = 0;
        void Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                    const Math::Vec2f& firstPos);

        template <class E, typename... Args>
        std::shared_ptr<E> AddElement(Args&&... args) {
            if(m_Elements.empty()) {
                m_GridDepth++;
                m_Elements.emplace_back(std::make_shared<E>(std::forward<Args>(args)...));
            } else {
                auto penultimateElement = m_Elements.back();
                m_Elements.emplace_back(std::make_shared<E>(std::forward<Args>(args)...));
                auto newElement = m_Elements.back();
                if(penultimateElement->ForceNextToNewRow() || newElement->OnNewRow()) {
                    m_GridDepth++;
                }
            }
            return std::dynamic_pointer_cast<E>(m_Elements.back());
        }

    protected:
        unsigned int m_GridDepth;
        std::vector<std::shared_ptr<GuiElement>> m_Elements;
        std::vector<Math::Vec2f> m_ElementPosCache;

        virtual std::shared_ptr<GuiElement> GetHoverElementInternal(const Math::Vec2f& mousePos,
                                                                    const Math::Vec2f& posOffset) const;
    };

    class GuiDropdown : public GuiButton, public GuiContainer {
        /*
         * Inherited behavior from GuiButton:
         * - Dropdown toggle is a button, but with maximum allowed width and an arrow icon
         */
    public:
        GuiDropdown(const std::string& text, unsigned int textSize);

        void Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize) override;
        bool IsHovering(const Math::Vec2f& mousePos) const override;
        float GetWidth() const override;
        unsigned int GetGridDepth() const override;
        std::shared_ptr<GuiElement> GetHoverElement(const Math::Vec2f& mousePos) const override;

    private:
        static constexpr float s_IconWidthHeight = GuiMetrics::WindowElementHeight;

        bool m_Open;
    };

    typedef unsigned int GuiWindowID;

    class GuiWindow : public GuiContainer {
    public:
        static std::shared_ptr<Shader> s_WindowShader;

        GuiWindow(const std::string& title, GuiWindowID id,
                  const Math::Vec2f& position, const Math::Vec2f& size);

        std::shared_ptr<GuiElement> GetHoverElement(const Math::Vec2f& mousePos) const override;

        void Render(bool selected);
        bool IsHoveringHeader(const Math::Vec2f& mousePos);
        bool IsHoveringResize(const Math::Vec2f& mousePos);
        bool IsHoveringWindow(const Math::Vec2f& mousePos);
        void Move(const Math::Vec2f& delta, const Math::Vec2f& minPos, const Math::Vec2f& maxPos);
        void Resize(const Math::Vec2f& delta, const Math::Vec2f& minSize, const Math::Vec2f& maxSize);
        Math::Vec2f GetPos() const;
        GuiWindowID GetID() const;

    private:
        Math::Vec2f m_Pos, m_Size;
        GuiWindowID m_ID;
        GuiText m_Title;
    };

}