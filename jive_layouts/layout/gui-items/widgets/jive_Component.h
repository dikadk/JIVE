#pragma once

namespace jive
{
    class Component : public GuiItemDecorator
    {
    public:
        explicit Component(std::unique_ptr<GuiItem> itemToDecorate);
        ~Component();

        juce::Component& getComponent();
        const juce::Component& getComponent() const;

    private:
        int getStyleFlags() const;

        juce::Component component;
        LookAndFeel lookAndFeel;

        Property<bool> hasShadow;
        Property<bool> isNative;
        Property<bool> isResizable;
        Property<bool> useCornerResizer;
        Property<float> minWidth;
        Property<float> minHeight;
        Property<float> maxWidth;
        Property<float> maxHeight;
        Property<bool> isDraggable;
        Property<bool> isFullScreen;
        Property<bool> isMinimised;
        Property<juce::String> name;
        Property<float> titleBarHeight;
        Property<juce::DocumentWindow::TitleBarButtons> titleBarButtons;
        Length width;
        Length height;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Component)
    };
} // namespace jive
