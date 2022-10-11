#pragma once

//======================================================================================================================
namespace jive
{
    //==================================================================================================================
    class BlockContainer : public GuiItemDecorator
    {
    public:
        //==============================================================================================================
        explicit BlockContainer(std::unique_ptr<GuiItem> itemToDecorate);

        //==============================================================================================================
        void updateLayout() override;

    protected:
        //==============================================================================================================
        void componentMovedOrResized(juce::Component& componentThatWasMovedOrResized,
                                     bool wasMoved,
                                     bool wasResized) override;

    private:
        //==============================================================================================================
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BlockContainer)
    };
} // namespace jive
