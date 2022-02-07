#include <catch2/catch_test_macros.hpp>
#include <jive_layouts/jive_layouts.h>

//======================================================================================================================
SCENARIO("GUI items can have children")
{
    GIVEN("a GUI item")
    {
        jive::GuiItem item{ std::make_unique<juce::Component>(), juce::ValueTree{ "Component" } };

        THEN("the item has no children")
        {
            REQUIRE(item.getNumChildren() == 0);
        }
        THEN("the item's component has no children")
        {
            REQUIRE(item.getComponent().getNumChildComponents() == 0);
        }

        WHEN("a child is added to the item")
        {
            item.addChild(std::make_unique<jive::GuiItem>(std::make_unique<juce::Component>(),
                                                          juce::ValueTree{ "Component" }));

            THEN("the item has a single child")
            {
                REQUIRE(item.getNumChildren() == 1);
            }
            THEN("the item's component has a single child")
            {
                REQUIRE(item.getComponent().getNumChildComponents() == 1);
            }
        }
        WHEN("multiple children are added to the item")
        {
            item.addChild(std::make_unique<jive::GuiItem>(std::make_unique<juce::Component>(),
                                                          juce::ValueTree{ "Component" }));
            item.addChild(std::make_unique<jive::GuiItem>(std::make_unique<juce::Component>(),
                                                          juce::ValueTree{ "Component" }));
            item.addChild(std::make_unique<jive::GuiItem>(std::make_unique<juce::Component>(),
                                                          juce::ValueTree{ "Component" }));

            THEN("the item has the expected number of children")
            {
                REQUIRE(item.getNumChildren() == 3);
            }
            THEN("the item's component has the same number of children")
            {
                REQUIRE(item.getComponent().getNumChildComponents() == 3);
            }
        }
    }
}

//======================================================================================================================
SCENARIO("GUI items can have an ID")
{
    GIVEN("a GUI item")
    {
        juce::ValueTree tree{ "Component" };
        jive::GuiItem item{ std::make_unique<juce::Component>(), tree };

        THEN("the item's ID is empty")
        {
            REQUIRE(item.getID().isNull());
        }
        AND_THEN("the ID of the item's component is empty")
        {
            REQUIRE(item.getComponent().getComponentID().isEmpty());
        }

        WHEN("the item's ID is changed")
        {
            tree.setProperty("id", "comp123", nullptr);

            THEN("the item's ID matches the one specified")
            {
                REQUIRE(item.getID() == juce::Identifier{ "comp123" });
            }
            AND_THEN("the ID of the item's component matches the one specified")
            {
                REQUIRE(item.getComponent().getComponentID() == "comp123");
            }
        }
    }
    GIVEN("a value-tree with a specified 'id' property")
    {
        juce::ValueTree tree{
            "Component",
            { { "id", "specified" } }
        };

        WHEN("a GUI item is constructed from the tree")
        {
            jive::GuiItem item{ std::make_unique<juce::Component>(), tree };

            THEN("the item's ID matches the one in the tree")
            {
                REQUIRE(item.getID() == juce::Identifier{ tree["id"] });
            }
            AND_THEN("the ID of the item's component matches the one in the tree")
            {
                REQUIRE(item.getComponent().getComponentID() == tree["id"].toString());
            }
        }
    }
}

//======================================================================================================================
SCENARIO("GUI items have a width and height")
{
    GIVEN("a top-level GUI item")
    {
        juce::ValueTree tree{ "Component" };
        jive::GuiItem item{ std::make_unique<juce::Component>(), tree };

        THEN("the item has an auto width and height")
        {
            REQUIRE(item.hasAutoWidth());
            REQUIRE(item.hasAutoHeight());
        }
        THEN("the item's component should have a size of 0x0")
        {
            REQUIRE(item.getComponent().getWidth() == 0.f);
            REQUIRE(item.getComponent().getHeight() == 0.f);
        }

        WHEN("the item's size is changed")
        {
            tree.setProperty("width", 100.11f, nullptr);
            tree.setProperty("height", 50.55f, nullptr);

            THEN("the item's size is the specified size rounded to the nearest "
                 "whole numbers")
            {
                REQUIRE(item.getWidth() == 100.f);
                REQUIRE(item.getHeight() == 51.f);
            }
            THEN("the item's component has the specified size (rounded to the nearest whole-number)")
            {
                REQUIRE(item.getComponent().getWidth() == 100);
                REQUIRE(item.getComponent().getHeight() == 51);
            }
        }
    }
}

//======================================================================================================================
SCENARIO("top-level GUI items use their component's size when resized")
{
    GIVEN("a top-level GUI item with a specified size")
    {
        juce::ValueTree tree{
            "Component",
            { { "width", 200 },
              { "height", 150 } }
        };
        jive::GuiItem item{ std::make_unique<juce::Component>(), tree };

        THEN("the item's component has the specified size")
        {
            REQUIRE(item.getComponent().getWidth() == 200);
            REQUIRE(item.getComponent().getHeight() == 150);
        }

        WHEN("the item's component is resized")
        {
            item.getComponent().setSize(400, 300);

            THEN("the item's size has changed to match its component")
            {
                REQUIRE(item.getWidth() == 400);
                REQUIRE(item.getHeight() == 300);
            }
        }
    }
}

//======================================================================================================================
SCENARIO("GUI items can have a name")
{
    GIVEN("a GUI item with no name")
    {
        juce::ValueTree tree{ "Component" };
        jive::GuiItem item{ std::make_unique<juce::Component>(), tree };

        THEN("the item has no name")
        {
            REQUIRE(item.getName().isEmpty());
        }
        THEN("the item's component has no name")
        {
            REQUIRE(item.getComponent().getName().isEmpty());
        }

        WHEN("the item's name changes")
        {
            tree.setProperty("name", "Zaphod Beeblebrox", nullptr);

            THEN("the item's name matches the one specified")
            {
                REQUIRE(item.getName() == "Zaphod Beeblebrox");
            }
            THEN("the name of the item's component matches the item's name")
            {
                REQUIRE(item.getComponent().getName() == item.getName());
            }
            THEN("the title of the item's component matches the item's name")
            {
                REQUIRE(item.getComponent().getTitle() == item.getName());
            }
        }

        WHEN("the name of the item's component changes")
        {
            item.getComponent().setName("Arthur Dent");

            THEN("the item's name matches its component")
            {
                REQUIRE(item.getName() == item.getComponent().getName());
            }
        }
    }

    GIVEN("a value tree with a specified 'name' property")
    {
        juce::ValueTree tree{ "Component" };
        tree.setProperty("name", "Ford Prefect", nullptr);

        WHEN("a GUI item is constructed from the tree")
        {
            jive::GuiItem item{ std::make_unique<juce::Component>(), tree };

            THEN("the item's name matches the tree's 'name' property")
            {
                REQUIRE(item.getName() == tree["name"].toString());
            }
            THEN("the name of the item's component matches the tree's 'name' property")
            {
                REQUIRE(item.getComponent().getName() == tree["name"].toString());
            }
            THEN("the title of the item's component matches the tree's 'name' property")
            {
                REQUIRE(item.getComponent().getTitle() == tree["name"].toString());
            }
        }
    }
}

//======================================================================================================================
SCENARIO("GUI items can be visible in invisible")
{
    GIVEN("a GUI item with no visibility specified")
    {
        juce::ValueTree tree{ "Component" };
        jive::GuiItem item{ std::make_unique<juce::Component>(), tree };

        THEN("the item is visible")
        {
            REQUIRE(item.isVisible());
        }
        THEN("the item's component is visible")
        {
            REQUIRE(item.getComponent().isVisible());
        }

        WHEN("the item's visibility changes")
        {
            tree.setProperty("visible", false, nullptr);

            THEN("the item's visible matches the visiblity specified")
            {
                REQUIRE_FALSE(item.isVisible());
            }
            THEN("the visibility of the item's component matches the visiblity of the item")
            {
                REQUIRE(item.getComponent().isVisible() == item.isVisible());
            }
        }

        WHEN("a visible child is added to the item")
        {
            juce::ValueTree childTree{ "Component" };
            childTree.setProperty("visible", true, nullptr);

            item.addChild(std::make_unique<jive::GuiItem>(std::make_unique<juce::Component>(), childTree, &item));

            THEN("the child is still visible")
            {
                REQUIRE(item.getChild(0).isVisible());
            }
            THEN("the child's component is still visible")
            {
                REQUIRE(item.getChild(0).getComponent().isVisible());
            }
        }

        WHEN("an invisible child is added to the item")
        {
            juce::ValueTree childTree{ "Component" };
            childTree.setProperty("visible", false, nullptr);

            item.addChild(std::make_unique<jive::GuiItem>(std::make_unique<juce::Component>(), childTree, &item));

            THEN("the child is still invisible")
            {
                REQUIRE_FALSE(item.getChild(0).isVisible());
            }
            THEN("the child's component is still invisible")
            {
                REQUIRE_FALSE(item.getChild(0).getComponent().isVisible());
            }
        }

        WHEN("the visiblity of the item's component changes")
        {
            item.getComponent().setVisible(false);

            THEN("the item's visibility matches the visibility of its component")
            {
                REQUIRE(item.isVisible() == item.getComponent().isVisible());
            }
        }
    }

    GIVEN("a value-tree with a specified 'visible' property")
    {
        juce::ValueTree tree{ "Component" };
        tree.setProperty("visible", false, nullptr);

        WHEN("a GUI item is constructed from the tree")
        {
            jive::GuiItem item{ std::make_unique<juce::Component>(), tree };

            THEN("the item's visiblity matches the tree's 'visible' property")
            {
                REQUIRE(item.isVisible() == static_cast<bool>(tree["visible"]));
            }
            THEN("the visibility of the item's component matches the visiblity of the item")
            {
                REQUIRE(item.getComponent().isVisible() == item.isVisible());
            }
        }
    }
}

//======================================================================================================================
SCENARIO("GUI items can be told to always be on top of their siblings")
{
    GIVEN("a GUI item")
    {
        juce::ValueTree tree{ "Component" };
        jive::GuiItem item{ std::make_unique<juce::Component>(), tree };

        THEN("the item won't always be on top")
        {
            REQUIRE_FALSE(item.isAlwaysOnTop());
        }
        THEN("the item's component won't always be on top")
        {
            REQUIRE_FALSE(item.getComponent().isAlwaysOnTop());
        }

        WHEN("the item is told to always be on top")
        {
            tree.setProperty("always-on-top", true, nullptr);

            THEN("the item will always be on top")
            {
                REQUIRE(item.isAlwaysOnTop());
            }
            THEN("the item's component will always be on top")
            {
                REQUIRE(item.getComponent().isAlwaysOnTop());
            }
        }
    }

    GIVEN("a value-tree with its 'always-on-top' property specified")
    {
        juce::ValueTree tree{ "Component" };
        tree.setProperty("always-on-top", true, nullptr);

        WHEN("a GUI item is constructed from the tree")
        {
            jive::GuiItem item{ std::make_unique<juce::Component>(), tree };

            THEN("whether the item will always be on top matches the tree's 'always-on-top' property")
            {
                REQUIRE(item.isAlwaysOnTop() == static_cast<bool>(tree["always-on-top"]));
            }
            THEN("whether the item's component will always be on top matches the tree's 'always-on-top' property")
            {
                REQUIRE(item.getComponent().isAlwaysOnTop() == static_cast<bool>(tree["always-on-top"]));
            }
        }
    }
}

//======================================================================================================================
SCENARIO("GUI items can be buffered to an image")
{
    GIVEN("a GUI item")
    {
        juce::ValueTree tree{ "Component" };
        jive::GuiItem item{ std::make_unique<juce::Component>(), tree };

        THEN("the item is not buffered to an image")
        {
            REQUIRE_FALSE(item.isBufferedToImage());
        }
        THEN("the item's component is not buffered to an image")
        {
            REQUIRE(item.getComponent().getCachedComponentImage() == nullptr);
        }

        WHEN("the item is told to buffer to an image")
        {
            tree.setProperty("buffered-to-image", true, nullptr);

            THEN("the item is buffered to an image")
            {
                REQUIRE(item.isBufferedToImage());
            }
            THEN("whether the item's component is buffered to an images matches whether the item is buffered to an "
                 "image")
            {
                REQUIRE((item.getComponent().getCachedComponentImage() != nullptr) == item.isBufferedToImage());
            }
        }
    }

    GIVEN("a value-tree with a specified 'buffered-to-image' property")
    {
        juce::ValueTree tree{ "Component" };
        tree.setProperty("buffered-to-image", true, nullptr);

        WHEN("a GUI item is constructed from the tree")
        {
            jive::GuiItem item{ std::make_unique<juce::Component>(), tree };

            THEN("whether to item is buffered to an image matches the tree's 'buffered-to-image' property")
            {
                REQUIRE(item.isBufferedToImage() == static_cast<bool>(tree["buffered-to-image"]));
            }
            THEN("whether the item's component is buffered to an images matches whether the item is buffered to an "
                 "image")
            {
                REQUIRE((item.getComponent().getCachedComponentImage() != nullptr) == item.isBufferedToImage());
            }
        }
    }
}

//======================================================================================================================
SCENARIO("GUI items can be opaque")
{
    GIVEN("a GUI item")
    {
        juce::ValueTree tree{ "Component" };
        jive::GuiItem item{ std::make_unique<juce::Component>(), tree };

        THEN("the item is not opaque")
        {
            REQUIRE_FALSE(item.isOpaque());
        }
        THEN("the item's component is not opaque")
        {
            REQUIRE_FALSE(item.getComponent().isOpaque());
        }

        WHEN("the item's opaque-ness changes")
        {
            tree.setProperty("opaque", true, nullptr);

            THEN("the item is opaque")
            {
                REQUIRE(item.isOpaque());
            }
            THEN("whether the item's component is opaque matches whether the item is opaque")
            {
                REQUIRE(item.getComponent().isOpaque() == item.isOpaque());
            }
        }
    }

    GIVEN("a value-tree with a specified 'opaque' property")
    {
        juce::ValueTree tree{ "Component" };
        tree.setProperty("opaque", true, nullptr);

        WHEN("a GUI item is constructed from the tree")
        {
            jive::GuiItem item{ std::make_unique<juce::Component>(), tree };

            THEN("whether the item is opaque matches the tree's 'opaque' property")
            {
                REQUIRE(item.isOpaque() == static_cast<bool>(tree["opaque"]));
            }
            THEN("whether the item's component is opaque matches whether the item is opaque")
            {
                REQUIRE(item.getComponent().isOpaque() == item.isOpaque());
            }
        }
    }
}

//======================================================================================================================
SCENARIO("GUi items have a focus order")
{
    GIVEN("a GUI item")
    {
        juce::ValueTree tree{ "Component" };
        jive::GuiItem item{ std::make_unique<juce::Component>(), tree };

        THEN("the item's focus order index is 0")
        {
            REQUIRE(item.getFocusOrder() == 0);
        }
        THEN("the focus order of the item's component matches that of the item")
        {
            REQUIRE(item.getComponent().getExplicitFocusOrder() == item.getFocusOrder());
        }

        WHEN("the item's focus order changes")
        {
            tree.setProperty("focus-order", 12, nullptr);

            THEN("the item's focus order has changed to match the specified value")
            {
                REQUIRE(item.getFocusOrder() == 12);
            }
            THEN("the focus order of the item's component matches that of the item")
            {
                REQUIRE(item.getComponent().getExplicitFocusOrder() == item.getFocusOrder());
            }
        }
    }

    GIVEN("a value-tree with a specified 'focus-order' property")
    {
        juce::ValueTree tree{ "Component" };
        tree.setProperty("focus-order", 4, nullptr);

        WHEN("a GUI item is constructed from the tree")
        {
            jive::GuiItem item{ std::make_unique<juce::Component>(), tree };

            THEN("the item's focus order matches the tree's 'focus-order' property")
            {
                REQUIRE(item.getFocusOrder() == static_cast<int>(tree["focus-order"]));
            }
            THEN("the focus order of the item's component matches that of the item")
            {
                REQUIRE(item.getComponent().getExplicitFocusOrder() == item.getFocusOrder());
            }
        }
    }
}

//======================================================================================================================
SCENARIO("GUI items can be focusable")
{
    GIVEN("a GUI item")
    {
        juce::ValueTree tree{ "Component" };
        jive::GuiItem item{ std::make_unique<juce::Component>(), tree };

        THEN("the item is not focusable")
        {
            REQUIRE_FALSE(item.isFocusable());
        }
        THEN("whether the item's component can be focused matches whether the item can be focused")
        {
            REQUIRE(item.getComponent().getWantsKeyboardFocus() == item.isFocusable());
        }

        WHEN("whether the item can be focused changes")
        {
            tree.setProperty("focusable", true, nullptr);

            THEN("whether the item can be focused matches the value specified")
            {
                REQUIRE(item.isFocusable());
            }
            THEN("whether the item's component can be focused matches whether the item can be focused")
            {
                REQUIRE(item.getComponent().getWantsKeyboardFocus() == item.isFocusable());
            }
        }
    }

    GIVEN("a value-tree with a specified 'focusable' property")
    {
        juce::ValueTree tree{ "Component" };
        tree.setProperty("focusable", true, nullptr);

        WHEN("a GUI item is constructed from the tree")
        {
            jive::GuiItem item{ std::make_unique<juce::Component>(), tree };

            THEN("whether the item can be focused matches the tree's 'focusable' property")
            {
                REQUIRE(item.isFocusable() == static_cast<bool>(tree["focusable"]));
            }
            THEN("whether the item's component can be focused matches whether the item can be focused")
            {
                REQUIRE(item.getComponent().getWantsKeyboardFocus() == item.isFocusable());
            }
        }
    }
}

//======================================================================================================================
SCENARIO("GUI items can optionally be clicked on to grab keyboard focus")
{
    GIVEN("a GUI item")
    {
        juce::ValueTree tree{ "Component" };
        jive::GuiItem item{ std::make_unique<juce::Component>(), tree };

        THEN("the item can be clicked on to grab keyboard focus")
        {
            REQUIRE(item.getClickingGrabsFocus());
        }
        THEN("whether the item's component can be clicked to grab focus matches whether the item can be clicked to "
             "grab focus")
        {
            REQUIRE(item.getComponent().getMouseClickGrabsKeyboardFocus() == item.getClickingGrabsFocus());
        }

        WHEN("whether the item can be clicked on to grab focus changes")
        {
            tree.setProperty("clicking-grabs-focus", false, nullptr);

            THEN("whether the item can be clicked to grab focus matches the value specified")
            {
                REQUIRE_FALSE(item.getClickingGrabsFocus());
            }
            THEN("whether the item's component can be clicked to grab focus matches whether the item can be clicked to "
                 "grab focus")
            {
                REQUIRE(item.getComponent().getMouseClickGrabsKeyboardFocus() == item.getClickingGrabsFocus());
            }
        }
    }

    GIVEN("a value-tree with a specified 'clicking-grabs-focus' property")
    {
        juce::ValueTree tree{ "Component" };
        tree.setProperty("clicking-grabs-focus", false, nullptr);

        WHEN("a GUI item is constructed from the tree")
        {
            jive::GuiItem item{ std::make_unique<juce::Component>(), tree };

            THEN("whether the item can be clicked to grab focus matches the tree's 'clicking-grabs-focus' property")
            {
                REQUIRE(item.getClickingGrabsFocus() == static_cast<bool>(tree["clicking-grabs-focus"]));
            }
            THEN("whether the item's component can be clicked to grab focus matches whether the item can be clicked to "
                 "grab focus")
            {
                REQUIRE(item.getComponent().getMouseClickGrabsKeyboardFocus() == item.getClickingGrabsFocus());
            }
        }
    }
}

//======================================================================================================================
SCENARIO("GUI items can have a focus outline")
{
    GIVEN("a GUI item")
    {
        juce::ValueTree tree{ "Component" };
        jive::GuiItem item{ std::make_unique<juce::Component>(), tree };

        THEN("the item will not have a focus outline")
        {
            REQUIRE_FALSE(item.hasFocusOutline());
        }
        THEN("whether the item's component will be outlined when focused matches whether the item will be")
        {
            REQUIRE(item.getComponent().hasFocusOutline() == item.hasFocusOutline());
        }

        WHEN("whether the item can will be outlined when focused changes")
        {
            tree.setProperty("focus-outline", true, nullptr);

            THEN("whether the item will be outlined when focused matches the value specified")
            {
                REQUIRE(item.hasFocusOutline());
            }
            THEN("whether the item's component will be outlined when focused matches whether the item will be")
            {
                REQUIRE(item.getComponent().hasFocusOutline() == item.hasFocusOutline());
            }
        }
    }

    GIVEN("a value-tree with a specified 'focus-outline' property")
    {
        juce::ValueTree tree{ "Component" };
        tree.setProperty("focus-outline", true, nullptr);

        WHEN("a GUI item is constructed from the tree")
        {
            jive::GuiItem item{ std::make_unique<juce::Component>(), tree };

            THEN("whether the item will have a focus outline matches the tree's 'focus-outline' property")
            {
                REQUIRE(item.hasFocusOutline() == static_cast<bool>(tree["focus-outline"]));
            }
            THEN("whether the item's component will be outlined when focused matches whether the item will be")
            {
                REQUIRE(item.getComponent().hasFocusOutline() == item.hasFocusOutline());
            }
        }
    }
}

//======================================================================================================================
SCENARIO("GUI items can be enabled or disabled")
{
    GIVEN("a GUI item")
    {
        juce::ValueTree tree{ "Component" };
        jive::GuiItem item{ std::make_unique<juce::Component>(), tree };

        THEN("the item is enabled")
        {
            REQUIRE(item.isEnabled());
        }
        THEN("the enablement of the item's component matches the item")
        {
            REQUIRE(item.getComponent().isEnabled() == item.isEnabled());
        }

        WHEN("the item's enablement changes")
        {
            tree.setProperty("enabled", false, nullptr);

            THEN("the item's enablement matches the value specified")
            {
                REQUIRE_FALSE(item.isEnabled());
            }
            THEN("the enablement of the item's component matches the item")
            {
                REQUIRE(item.getComponent().isEnabled() == item.isEnabled());
            }
        }

        WHEN("the enablement of the item's component changes")
        {
            item.getComponent().setEnabled(false);

            THEN("the item's enablement matches its component")
            {
                REQUIRE(item.isEnabled() == item.getComponent().isEnabled());
            }
        }
    }

    GIVEN("a value-tree with a specified 'enabled' property")
    {
        juce::ValueTree tree{ "Component" };
        tree.setProperty("enabled", false, nullptr);

        WHEN("a GUI item is constructed from the tree")
        {
            jive::GuiItem item{ std::make_unique<juce::Component>(), tree };

            THEN("the enablement of the item matches the tree's 'enabled' property")
            {
                REQUIRE(item.isEnabled() == static_cast<bool>(tree["enabled"]));
            }
            THEN("the enablement of the item's component matches the item")
            {
                REQUIRE(item.getComponent().isEnabled() == item.isEnabled());
            }
        }
    }
}
