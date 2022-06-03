#pragma once

#include <binary_data.h>
#include <jive_demo/gui/Layouts.h>
#include <jive_demo/gui/aliases/Aliases.h>
#include <jive_demo/gui/pages/HomePagePresenter.h>

#include <jive_layouts/jive_layouts.h>

namespace jive_demo
{
    class WindowPresenter
    {
    public:
        WindowPresenter()
            : window{
                parseLayout(binary_data::window_xml, binary_data::window_xmlSize),
            }
        {
            window.setProperty("style",
                               juce::String::createStringFromData(binary_data::style_json,
                                                                  binary_data::style_jsonSize),
                               nullptr);

            window.appendChild(homePage.present(), nullptr);
        }

        std::unique_ptr<jive::GuiItem> present()
        {
            jive::Interpreter interpreter;

            for (const auto& [name, tree] : getAliases())
                interpreter.setAlias(name, tree);

            return interpreter.interpret(window);
        }

    private:
        juce::ValueTree window;

        HomePagePresenter homePage;
    };
} // namespace jive_demo