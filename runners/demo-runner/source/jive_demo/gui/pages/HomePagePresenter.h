#pragma once

#include <binary_data.h>
#include <jive_core/jive_core.h>
#include <jive_demo/gui/Layouts.h>

namespace jive_demo
{
    class HomePagePresenter
    {
    public:
        HomePagePresenter()
            : homePage{
                parseLayout(binary_data::home_page_xml,
                            binary_data::home_page_xmlSize),
            }
            , onHomeButtonClicked{
                jive::find(homePage, [](const auto& element) {
                    return element["id"].toString() == "home";
                }),
                "on-click",
            }
        {
            onHomeButtonClicked.onTrigger = []() {
                DBG("HOME CLICK!");
            };
        }

        juce::ValueTree present()
        {
            return homePage;
        }

    private:
        juce::ValueTree homePage;

        jive::Event onHomeButtonClicked;
    };
} // namespace jive_demo