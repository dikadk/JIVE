#include <jive_layouts/jive_layouts.h>

#ifndef JUCE_APPLICATION_NAME
    #define JUCE_APPLICATION_NAME ""
#endif

namespace jive
{
    Component::Component(std::unique_ptr<GuiItem> itemToDecorate)
        : GuiItemDecorator{ std::move(itemToDecorate) }
        , hasShadow{ state, "shadow", true }
        , isNative{ state, "native", true }
        , isResizable{ state, "resizable", true }
        , useCornerResizer{ state, "corner-resizer" }
        , minWidth{ state, "min-width", 1.0f }
        , minHeight{ state, "min-height", 1.0f }
        , maxWidth{ state, "max-width", static_cast<float>(std::numeric_limits<juce::int16>::max()) }
        , maxHeight{ state, "max-height", static_cast<float>(std::numeric_limits<juce::int16>::max()) }
        , isDraggable{ state, "draggable", true }
        , isFullScreen{ state, "full-screen" }
        , isMinimised{ state, "minimised" }
        , name{ state, "name", JUCE_APPLICATION_NAME }
        , titleBarHeight{ state, "title-bar-height", 26 }
        , titleBarButtons{ state, "title-bar-buttons", juce::DocumentWindow::allButtons }
        , width{ state, "width" }
        , height{ state, "height" }
    {
        hasShadow.onValueChange = [this]() {
            //getComponent().setDropShadowEnabled(hasShadow);
        };
        //getComponent().setDropShadowEnabled(hasShadow);

        isNative.onValueChange = [this]() {
            //getComponent().setUsingNativeTitleBar(isNative);
        };
        //getComponent().setUsingNativeTitleBar(isNative);

        isResizable.onValueChange = [this]() {
            //getComponent().setResizable(isResizable, useCornerResizer);
        };
        useCornerResizer.onValueChange = [this]() {
            //getComponent().setResizable(isResizable, useCornerResizer);
        };
        //getComponent().setResizable(isResizable, useCornerResizer);

        /*minWidth.onValueChange = [this]() {
            getComponent().setResizeLimits(static_cast<int>(std::ceil(minWidth)),
                                        static_cast<int>(std::ceil(minHeight)),
                                        static_cast<int>(std::floor(maxWidth)),
                                        static_cast<int>(std::floor(maxHeight)));
        };
        minHeight.onValueChange = [this]() {
            getComponent().setResizeLimits(static_cast<int>(std::ceil(minWidth)),
                                        static_cast<int>(std::ceil(minHeight)),
                                        static_cast<int>(std::floor(maxWidth)),
                                        static_cast<int>(std::floor(maxHeight)));
        };
        maxWidth.onValueChange = [this]() {
            getComponent().setResizeLimits(static_cast<int>(std::ceil(minWidth)),
                                        static_cast<int>(std::ceil(minHeight)),
                                        static_cast<int>(std::floor(maxWidth)),
                                        static_cast<int>(std::floor(maxHeight)));
        };
        maxHeight.onValueChange = [this]() {
            getComponent().setResizeLimits(static_cast<int>(std::ceil(minWidth)),
                                        static_cast<int>(std::ceil(minHeight)),
                                        static_cast<int>(std::floor(maxWidth)),
                                        static_cast<int>(std::floor(maxHeight)));
        };
        getComponent().setResizeLimits(static_cast<int>(std::ceil(minWidth)),
                                    static_cast<int>(std::ceil(minHeight)),
                                    static_cast<int>(std::floor(maxWidth)),
                                    static_cast<int>(std::floor(maxHeight)));
*/
        /*isDraggable.onValueChange = [this]() {
            getComponent().setDraggable(isDraggable);
        };
        getComponent().setDraggable(isDraggable);

        isFullScreen.onValueChange = [this]() {
            getComponent().setFullScreen(isFullScreen);
        };
        getComponent().setFullScreen(isFullScreen);

        isMinimised.onValueChange = [this]() {
            getComponent().setMinimised(isMinimised);
        };
        getComponent().setMinimised(isMinimised);

        name.onValueChange = [this]() {
            getComponent().setName(name);
        };
        getComponent().setName(name);

        titleBarHeight.onValueChange = [this]() {
            getComponent().setTitleBarHeight(juce::roundToInt(titleBarHeight));
        };
        getComponent().setTitleBarHeight(juce::roundToInt(titleBarHeight));*/

#if JUCE_MAC
        static constexpr auto leftAlignButtons = true;
#else
        static constexpr auto leftAlignButtons = false;
#endif

        /*titleBarButtons.onValueChange = [this]() {
            getComponent().setTitleBarButtonsRequired(titleBarButtons, leftAlignButtons);
        };
        getComponent().setTitleBarButtonsRequired(titleBarButtons, leftAlignButtons);*/

        const auto& boxModel = toType<CommonGuiItem>()->boxModel;
        const auto windowWidth = juce::roundToInt(boxModel.getWidth());
        const auto windowHeight = juce::roundToInt(boxModel.getHeight());

        static constexpr auto resizeWindowWhenComponentSizeChanges = true;
        getComponent().centreWithSize(windowWidth, windowHeight);
        getComponent().setLookAndFeel(&lookAndFeel);
    }

    Component::~Component()
    {
        getComponent().setLookAndFeel(nullptr);
    }

    juce::Component& Component::getComponent()
    {
        return component;
    }

    const juce::Component& Component::getComponent() const
    {
        return component;
    }
} // namespace jive

#if JIVE_UNIT_TESTS
class WindowTest : public juce::UnitTest
{
public:
    WindowTest()
        : juce::UnitTest{ "jive::Component", "jive" }
    {
    }

    void runTest() final
    {
        testShadow();
        testNative();
        testResizable();
        testSizeLimits();
        testDraggable();
        testFullScreen();
        testMinimised();
        testName();
        testTitleBarHeight();
    }

private:
    std::unique_ptr<jive::Component> createWindow(juce::ValueTree tree)
    {
        jive::Interpreter interpreter;

        return std::unique_ptr<jive::Component>{
            dynamic_cast<jive::GuiItemDecorator*>(interpreter.interpret(tree).release())
                ->toType<jive::Component>(),
        };
    }

    void testShadow()
    {
        beginTest("shadow");

        {
            juce::ValueTree tree{
                "Component",
                {
                    { "width", 100 },
                    { "height", 100 },
                },
            };
            auto item = createWindow(tree);
            expect(item->getComponent().isDropShadowEnabled());
            expect(item->getComponent().isOnDesktop());
            expect(item->getComponent().isVisible());
            expect(item->getComponent().isShowing());

            tree.setProperty("shadow", false, nullptr);
            expect(!item->getComponent().isDropShadowEnabled());
        }
        {
            juce::ValueTree tree{
                "Component",
                {
                    { "shadow", false },
                    { "width", 100 },
                    { "height", 100 },
                },
            };
            auto item = createWindow(tree);
            expect(!item->getComponent().isDropShadowEnabled());
        }
    }

    void testNative()
    {
        beginTest("native");

        {
            juce::ValueTree state{
                "Component",
                {
                    { "width", 100 },
                    { "height", 150 },
                },
            };
            jive::Interpreter interpreter;
            auto item = interpreter.interpret(state);
            expect(dynamic_cast<jive::Component&>(*item).getComponent().isUsingNativeTitleBar());
            expectEquals(item->getComponent()->getWidth(), 100);
            expectEquals(item->getComponent()->getHeight(), 150);
        }
        {
            juce::ValueTree state{
                "Component",
                {
                    { "native", false },
                    { "width", 338 },
                    { "height", 894 },
                },
            };
            jive::Interpreter interpreter;
            auto item = interpreter.interpret(state);
            auto& Component = dynamic_cast<jive::Component&>(*item).getComponent();
            expect(!Component.isUsingNativeTitleBar());
            expectEquals(item->getComponent()->getWidth(),
                         338 - Component.getBorderThickness().getLeftAndRight());
            expectEquals(item->getComponent()->getHeight(),
                         894 - Component.getBorderThickness().getTopAndBottom() - Component.getTitleBarHeight());
        }
    }

    void testResizable()
    {
        beginTest("resizable");

        {
            juce::ValueTree tree{
                "Component",
                {
                    { "width", 100 },
                    { "height", 100 },
                },
            };
            auto item = createWindow(tree);
            expect(item->getComponent().isResizable());

            tree.setProperty("resizable", false, nullptr);
            expect(!item->getComponent().isResizable());
        }
        {
            juce::ValueTree tree{
                "Component",
                {
                    { "resizable", false },
                    { "width", 100 },
                    { "height", 100 },
                },
            };
            auto item = createWindow(tree);
            expect(!item->getComponent().isResizable());
        }
    }

    void testSizeLimits()
    {
        beginTest("size limits");

        {
            juce::ValueTree tree{
                "Component",
                {
                    { "width", 100 },
                    { "height", 100 },
                },
            };
            auto item = createWindow(tree);
            expectEquals(item->getComponent().getConstrainer()->getMinimumWidth(), 1);
            expectEquals(item->getComponent().getConstrainer()->getMinimumHeight(), 1);
            expectEquals<int>(item->getComponent().getConstrainer()->getMaximumWidth(),
                              std::numeric_limits<juce::int16>::max());
            expectEquals<int>(item->getComponent().getConstrainer()->getMaximumHeight(),
                              std::numeric_limits<juce::int16>::max());

            tree.setProperty("min-width", 256, nullptr);
            expectEquals(item->getComponent().getConstrainer()->getMinimumWidth(), 256);

            tree.setProperty("min-height", 256, nullptr);
            expectEquals(item->getComponent().getConstrainer()->getMinimumHeight(), 256);

            tree.setProperty("max-width", 512, nullptr);
            expectEquals(item->getComponent().getConstrainer()->getMaximumWidth(), 512);

            tree.setProperty("max-height", 512, nullptr);
            expectEquals(item->getComponent().getConstrainer()->getMaximumHeight(), 512);
        }
        {
            juce::ValueTree tree{
                "Component",
                {
                    { "min-width", 246 },
                    { "min-height", 369 },
                    { "max-width", 1122 },
                    { "max-height", 3344 },
                    { "width", 100 },
                    { "height", 100 },
                },
            };
            auto item = createWindow(tree);
            expectEquals(item->getComponent().getConstrainer()->getMinimumWidth(), 246);
            expectEquals(item->getComponent().getConstrainer()->getMinimumHeight(), 369);
            expectEquals(item->getComponent().getConstrainer()->getMaximumWidth(), 1122);
            expectEquals(item->getComponent().getConstrainer()->getMaximumHeight(), 3344);
        }
    }

    void testDraggable()
    {
        beginTest("draggable");

        {
            juce::ValueTree tree{
                "Component",
                {
                    { "width", 100 },
                    { "height", 100 },
                },
            };
            auto item = createWindow(tree);
            expect(item->getComponent().isDraggable());

            tree.setProperty("draggable", false, nullptr);
            expect(!item->getComponent().isDraggable());
        }
        {
            juce::ValueTree tree{
                "Component",
                {
                    { "draggable", false },
                    { "width", 100 },
                    { "height", 100 },
                },
            };
            auto item = createWindow(tree);
            expect(!item->getComponent().isDraggable());
        }
    }

    void testFullScreen()
    {
        beginTest("full-screen");

        juce::ValueTree tree{
            "Component",
            {
                { "width", 100 },
                { "height", 100 },
            },
        };
        auto item = createWindow(tree);
        expect(!item->getComponent().isFullScreen());
    }

    void testMinimised()
    {
        beginTest("minimised");

        juce::ValueTree tree{
            "Component",
            {
                { "width", 100 },
                { "height", 100 },
            },
        };
        auto item = createWindow(tree);
        expect(!item->getComponent().isMinimised());
    }

    void testName()
    {
        beginTest("name");

        {
            juce::ValueTree tree{
                "Component",
                {
                    { "width", 100 },
                    { "height", 100 },
                },
            };
            auto item = createWindow(tree);
            expectEquals(item->getComponent().getName(), juce::String{ JUCE_APPLICATION_NAME });

            tree.setProperty("name", "foo", nullptr);
            expectEquals(item->getComponent().getName(), juce::String{ "foo" });
        }
        {
            juce::ValueTree tree{
                "Component",
                {
                    { "name", "foo" },
                    { "width", 100 },
                    { "height", 100 },
                },
            };
            auto item = createWindow(tree);
            expectEquals(item->getComponent().getName(), juce::String{ "foo" });
        }
    }

    void testTitleBarHeight()
    {
        beginTest("title-bar-height");

        {
            juce::ValueTree tree{
                "Component",
                {
                    { "native", false },
                    { "width", 100 },
                    { "height", 100 },
                },
            };
            auto item = createWindow(tree);
            expectEquals(item->getComponent().getTitleBarHeight(), 26);

            tree.setProperty("title-bar-height", 32, nullptr);
            expectEquals(item->getComponent().getTitleBarHeight(), 32);
        }
        {
            juce::ValueTree tree{
                "Component",
                {
                    { "native", false },
                    { "title-bar-height", 100 },
                    { "width", 100 },
                    { "height", 222 },
                },
            };
            auto item = createWindow(tree);
            expectEquals(item->getComponent().getTitleBarHeight(), 100);
        }
    }
};

static WindowTest windowTest;
#endif
