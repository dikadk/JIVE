#pragma once

namespace jive
{
    class Button
        : public GuiItemDecorator
        , private juce::Button::Listener
    {
    public:
        enum class TriggerEvent
        {
            mouseUp,
            mouseDown
        };

        explicit Button(std::unique_ptr<GuiItem> itemToDecorate);
        ~Button() override;

        bool isContainer() const override;

        juce::Button& getButton();
        const juce::Button& getButton() const;

    private:
        void buttonClicked(juce::Button* button) final;

        void updateTriggerEvent();

        Property<bool> toggleable;
        Property<bool> toggled;
        Property<bool> toggleOnClick;
        Property<int> radioGroup;
        Property<TriggerEvent> triggerEvent;
        Property<juce::String> tooltip;
        Property<juce::FlexBox::Direction> flexDirection;
        Property<juce::FlexBox::JustifyContent> justifyContent;
        Property<juce::BorderSize<float>> padding;
        Property<float> minWidth;
        Property<float> minHeight;

        Event onClick;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Button)
    };
} // namespace jive

namespace juce
{
    template <>
    class VariantConverter<jive::Button::TriggerEvent>
    {
    public:
        static jive::Button::TriggerEvent fromVar(const var& v);
        static var toVar(const jive::Button::TriggerEvent& event);

    private:
        static const Array<var> options;
    };
} // namespace juce
