#include <jive_layouts/jive_layouts.h>

//======================================================================================================================
namespace jive
{
    //==================================================================================================================
    Spinner::Spinner(std::unique_ptr<GuiItem> itemToDecorate)
        : Slider{ std::move(itemToDecorate), 50.0f, 20.0f }
        , draggable{ state, "draggable" }
    {
        draggable.onValueChange = [this]() {
            getSlider().setIncDecButtonsMode(draggable ? juce::Slider::incDecButtonsDraggable_AutoDirection : juce::Slider::incDecButtonsNotDraggable);
        };
        getSlider().setIncDecButtonsMode(draggable ? juce::Slider::incDecButtonsDraggable_AutoDirection : juce::Slider::incDecButtonsNotDraggable);

        updateStyle();
    }

    //==================================================================================================================
    juce::Slider::SliderStyle Spinner::getStyleForOrientation(Orientation /* orientation */)
    {
        return juce::Slider::IncDecButtons;
    }
} // namespace jive

//======================================================================================================================
#if JIVE_UNIT_TESTS
class SpinnerTest : public juce::UnitTest
{
public:
    SpinnerTest()
        : juce::UnitTest{ "jive::Spinner", "jive" }
    {
    }

    void runTest() final
    {
        testOrientation();
        testAutoSize();
    }

private:
    std::unique_ptr<jive::Spinner> createSpinner(juce::ValueTree tree)
    {
        jive::Interpreter interpreter;

        return std::make_unique<jive::Spinner>(interpreter.interpret(tree));
    }

    void testOrientation()
    {
        beginTest("orientation");

        juce::ValueTree tree{
            "Spinner",
            {
                { "orientation", "horizontal" },
            },
        };
        auto item = createSpinner(tree);
        expectEquals(item->getSlider().getSliderStyle(), juce::Slider::IncDecButtons);

        tree.setProperty("orientation", "vertical", nullptr);
        expectEquals(item->getSlider().getSliderStyle(), juce::Slider::IncDecButtons);
    }

    void testAutoSize()
    {
        beginTest("auto size");

        juce::ValueTree tree{ "Spinner" };
        auto item = createSpinner(tree);
        expectEquals(item->boxModel.getWidth(), 50.0f);
        expectEquals(item->boxModel.getHeight(), 20.0f);

        tree.setProperty("width", 38.0f, nullptr);
        expectEquals(item->boxModel.getWidth(), 38.0f);

        tree.setProperty("height", 73.0f, nullptr);
        expectEquals(item->boxModel.getHeight(), 73.0f);
    }
};

static SpinnerTest spinnerTest;
#endif
