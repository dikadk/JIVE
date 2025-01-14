#pragma once

namespace jive
{
    class GridItem : public GuiItemDecorator
    {
    public:
        explicit GridItem(std::unique_ptr<GuiItem> itemToDecorate);

        operator juce::GridItem();

    private:
        Property<int> order;
        Property<juce::GridItem::JustifySelf> justifySelf;
        Property<juce::GridItem::AlignSelf> alignSelf;
        Property<juce::GridItem::StartAndEndProperty> gridColumn;
        Property<juce::GridItem::StartAndEndProperty> gridRow;
        Property<juce::String> gridArea;
        Property<float> maxWidth;
        Property<float> maxHeight;
        Length width;
        Length height;
        Length minWidth;
        Length minHeight;

        const BoxModel& boxModel;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GridItem)
    };
} // namespace jive
