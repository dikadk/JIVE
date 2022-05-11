#pragma once

//======================================================================================================================
namespace jive
{
    //==================================================================================================================
    class BlockItem : public GuiItemDecorator
    {
    public:
        //==============================================================================================================
        explicit BlockItem(std::unique_ptr<GuiItem> itemToDecorate);

        //==============================================================================================================
        float getWidth() const override;
        float getHeight() const override;

    private:
        //==============================================================================================================
        int calculateX() const;
        int calculateY() const;
        juce::Point<int> calculatePosition() const;

        //==============================================================================================================
        TypedValue<Width> x;
        TypedValue<Height> y;
        TypedValue<Width> centreX;
        TypedValue<Height> centreY;

        //==============================================================================================================
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BlockItem)
    };
} // namespace jive
