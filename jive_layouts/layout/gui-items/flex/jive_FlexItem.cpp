#include <jive_layouts/jive_layouts.h>

//======================================================================================================================
namespace jive
{
    //==================================================================================================================
    FlexItem::FlexItem(std::unique_ptr<GuiItem> itemToDecorate)
        : GuiItemDecorator{ std::move(itemToDecorate) }
        , flexItemOrder{ state, "order" }
        , flexItemGrow{ state, "flex-grow" }
        , flexItemShrink{ state, "flex-shrink", 1 }
        , flexItemBasis{ state, "flex-basis" }
        , flexItemAlignSelf{ state, "align-self", juce::FlexItem::AlignSelf::autoAlign }
        , width{ state, "width", "auto" }
        , height{ state, "height", "auto" }
    {
        jassert(getParent() != nullptr);

        const auto updateParentLayout = [this]() {
            getParent()->layOutChildren();
        };
        flexItemOrder.onValueChange = updateParentLayout;
        flexItemGrow.onValueChange = updateParentLayout;
        flexItemShrink.onValueChange = updateParentLayout;
        flexItemBasis.onValueChange = updateParentLayout;
        flexItemAlignSelf.onValueChange = updateParentLayout;
    }

    //==================================================================================================================
    juce::FlexItem::Margin boxModelToFlexMargin(const juce::BorderSize<float>& border)
    {
        return juce::FlexItem::Margin{
            border.getTop(),
            border.getRight(),
            border.getBottom(),
            border.getLeft()
        };
    }

    FlexItem::operator juce::FlexItem()
    {
        juce::FlexItem flexItem{ *component };

        if (!getTopLevelDecorator().isContainer())
        {
            flexItem.width = static_cast<float>(boxModel.getWidth());
            flexItem.height = static_cast<float>(boxModel.getHeight());
        }
        else
        {
            const auto parentContentBounds = getParent()->boxModel.getContentBounds();

            if (!boxModel.hasAutoWidth())
                flexItem.width = width.toPixels(parentContentBounds);
            else
                flexItem.minWidth = static_cast<float>(boxModel.getWidth());

            if (!boxModel.hasAutoHeight())
                flexItem.height = height.toPixels(parentContentBounds);
            else
                flexItem.minHeight = static_cast<float>(boxModel.getHeight());
        }

        flexItem.order = flexItemOrder;
        flexItem.flexGrow = flexItemGrow;
        flexItem.flexShrink = flexItemShrink;
        flexItem.flexBasis = flexItemBasis;
        flexItem.alignSelf = flexItemAlignSelf;
        flexItem.margin = boxModelToFlexMargin(boxModel.getMargin());

        return flexItem;
    }
} // namespace jive

//======================================================================================================================
#if JIVE_UNIT_TESTS
class FlexItemUnitTest : public juce::UnitTest
{
public:
    FlexItemUnitTest()
        : juce::UnitTest{ "jive::FlexItem", "jive" }
    {
    }

    void runTest() final
    {
        testComponent();
        testOrder();
        testFlexGrow();
        testFlexShrink();
        testFlexBasis();
        testAlignSelf();
        testSize();
        testMargin();
        testDecoratedItems();
    }

private:
    void testComponent()
    {
        beginTest("component");

        jive::Interpreter interpreter;
        juce::ValueTree state{
            "Component",
            {
                { "width", 222 },
                { "height", 333 },
            },
            {
                juce::ValueTree{ "Component" },
            },
        };
        auto parent = interpreter.interpret(state);
        auto& item = parent->getChild(0);
        const auto flexItem = static_cast<juce::FlexItem>(*dynamic_cast<jive::GuiItemDecorator&>(item)
                                                               .toType<jive::FlexItem>());
        expect(flexItem.associatedComponent == item.getComponent().get());
    }

    void testOrder()
    {
        beginTest("order");

        jive::Interpreter interpreter;
        juce::ValueTree state{
            "Component",
            {
                { "width", 222 },
                { "height", 333 },
            },
            {
                juce::ValueTree{ "Component" },
            },
        };
        auto parent = interpreter.interpret(state);
        auto& item = parent->getChild(0);
        auto flexItem = static_cast<juce::FlexItem>(*dynamic_cast<jive::GuiItemDecorator&>(item)
                                                         .toType<jive::FlexItem>());

        expect(flexItem.order == 0);

        state.getChild(0).setProperty("order", 10, nullptr);
        flexItem = static_cast<juce::FlexItem>(*dynamic_cast<jive::GuiItemDecorator&>(item)
                                                    .toType<jive::FlexItem>());

        expect(flexItem.order == 10);
    }

    void testFlexGrow()
    {
        beginTest("flex-grow");

        jive::Interpreter interpreter;
        juce::ValueTree state{
            "Component",
            {
                { "width", 222 },
                { "height", 333 },
            },
            {
                juce::ValueTree{ "Component" },
            },
        };
        auto parent = interpreter.interpret(state);
        auto& item = parent->getChild(0);
        auto flexItem = static_cast<juce::FlexItem>(*dynamic_cast<jive::GuiItemDecorator&>(item)
                                                         .toType<jive::FlexItem>());

        expect(flexItem.flexGrow == 0.f);

        state.getChild(0).setProperty("flex-grow", 5.f, nullptr);
        flexItem = static_cast<juce::FlexItem>(*dynamic_cast<jive::GuiItemDecorator&>(item)
                                                    .toType<jive::FlexItem>());

        expect(flexItem.flexGrow == 5.f);
    }

    void testFlexShrink()
    {
        beginTest("flex-shrink");

        jive::Interpreter interpreter;
        juce::ValueTree state{
            "Component",
            {
                { "width", 222 },
                { "height", 333 },
            },
            {
                juce::ValueTree{ "Component" },
            },
        };
        auto parent = interpreter.interpret(state);
        auto& item = parent->getChild(0);
        auto flexItem = static_cast<juce::FlexItem>(*dynamic_cast<jive::GuiItemDecorator&>(item)
                                                         .toType<jive::FlexItem>());

        expect(flexItem.flexShrink == 1.f);

        state.getChild(0).setProperty("flex-shrink", 3.4f, nullptr);
        flexItem = static_cast<juce::FlexItem>(*dynamic_cast<jive::GuiItemDecorator&>(item)
                                                    .toType<jive::FlexItem>());

        expect(flexItem.flexShrink == 3.4f);
    }

    void testFlexBasis()
    {
        beginTest("flex-basis");

        jive::Interpreter interpreter;
        juce::ValueTree state{
            "Component",
            {
                { "width", 222 },
                { "height", 333 },
            },
            {
                juce::ValueTree{ "Component" },
            },
        };
        auto parent = interpreter.interpret(state);
        auto& item = parent->getChild(0);
        auto flexItem = static_cast<juce::FlexItem>(*dynamic_cast<jive::GuiItemDecorator&>(item)
                                                         .toType<jive::FlexItem>());

        expect(flexItem.flexBasis == 0.f);

        state.getChild(0).setProperty("flex-basis", 4.f, nullptr);
        flexItem = static_cast<juce::FlexItem>(*dynamic_cast<jive::GuiItemDecorator&>(item)
                                                    .toType<jive::FlexItem>());

        expect(flexItem.flexBasis == 4.f);
    }

    void testAlignSelf()
    {
        beginTest("align-self");

        jive::Interpreter interpreter;
        juce::ValueTree state{
            "Component",
            {
                { "width", 222 },
                { "height", 333 },
            },
            {
                juce::ValueTree{ "Component" },
            },
        };
        auto parent = interpreter.interpret(state);
        auto& item = parent->getChild(0);
        auto flexItem = static_cast<juce::FlexItem>(*dynamic_cast<jive::GuiItemDecorator&>(item)
                                                         .toType<jive::FlexItem>());

        expect(flexItem.alignSelf == juce::FlexItem::AlignSelf::autoAlign);

        state.getChild(0).setProperty("align-self", "centre", nullptr);
        flexItem = static_cast<juce::FlexItem>(*dynamic_cast<jive::GuiItemDecorator&>(item)
                                                    .toType<jive::FlexItem>());

        expect(flexItem.alignSelf == juce::FlexItem::AlignSelf::center);
    }

    void testSize()
    {
        beginTest("size");

        jive::Interpreter interpreter;
        juce::ValueTree state{
            "Component",
            {
                { "width", 100 },
                { "height", 200 },
            },
            {
                juce::ValueTree{ "Component" },
            },
        };
        auto parent = interpreter.interpret(state);
        auto& item = parent->getChild(0);
        auto flexItem = static_cast<juce::FlexItem>(*dynamic_cast<jive::GuiItemDecorator&>(item)
                                                         .toType<jive::FlexItem>());
        expectEquals(flexItem.width, juce::FlexItem{}.width);
        expectEquals(flexItem.height, juce::FlexItem{}.height);

        state.getChild(0).setProperty("width", 50.f, nullptr);
        state.getChild(0).setProperty("height", 175.f, nullptr);
        flexItem = static_cast<juce::FlexItem>(*dynamic_cast<jive::GuiItemDecorator&>(item)
                                                    .toType<jive::FlexItem>());

        expect(flexItem.width == 50.f);
        expect(flexItem.height == 175.f);
    }

    void testMargin()
    {
        beginTest("margin");

        jive::Interpreter interpreter;
        juce::ValueTree state{
            "Component",
            {
                { "width", 100 },
                { "height", 200 },
            },
            {
                juce::ValueTree{ "Component" },
            },
        };
        auto parent = interpreter.interpret(state);
        auto& item = parent->getChild(0);
        auto flexItem = static_cast<juce::FlexItem>(*dynamic_cast<jive::GuiItemDecorator&>(item)
                                                         .toType<jive::FlexItem>());

        expect(flexItem.margin.top == 0.f);
        expect(flexItem.margin.right == 0.f);
        expect(flexItem.margin.bottom == 0.f);
        expect(flexItem.margin.left == 0.f);

        state.getChild(0).setProperty("margin", "1 2 3 4", nullptr);
        flexItem = static_cast<juce::FlexItem>(*dynamic_cast<jive::GuiItemDecorator&>(item)
                                                    .toType<jive::FlexItem>());

        expect(flexItem.margin.top == 1.f);
        expect(flexItem.margin.right == 2.f);
        expect(flexItem.margin.bottom == 3.f);
        expect(flexItem.margin.left == 4.f);
    }

    void testDecoratedItems()
    {
        beginTest("decorated-items");

        struct StubItem : public jive::GuiItemDecorator
        {
            explicit StubItem(std::unique_ptr<jive::GuiItem> itemToDecorate)
                : jive::GuiItemDecorator{ std::move(itemToDecorate) }
            {
                boxModel.setWidth(123.45f);
                boxModel.setHeight(678.9f);
            }

            bool isContainer() const final
            {
                return false;
            }
        };

        jive::Interpreter interpreter;
        interpreter.getComponentFactory().set("Stub", []() {
            return std::make_unique<juce::Component>();
        });
        interpreter.addDecorator<StubItem>("Stub");

        juce::ValueTree parentState{
            "Component",
            {
                { "id", "parent" },
                { "display", "flex" },
                { "width", 333 },
                { "height", 999 },
            },
            {
                juce::ValueTree{ "Stub" },
            },
        };
        auto parent = interpreter.interpret(parentState);
        auto& item = parent->getChild(0);
        const auto flexItem = static_cast<juce::FlexItem>(*dynamic_cast<jive::GuiItemDecorator&>(item)
                                                               .toType<jive::FlexItem>());
        expectEquals(flexItem.width, 123.0f);
        expectEquals(flexItem.height, 679.0f);
    }
};

static FlexItemUnitTest flexItemUnitTest;
#endif
