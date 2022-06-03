#pragma once

#include <jive_layouts/jive_layouts.h>

#include <binary_data.h>
#include <jive_demo/gui/Layouts.h>

namespace jive_demo
{
    juce::var parseJSON(const void* const stringData, int stringDataSize)
    {
        return jive::parseJSON(juce::String::createStringFromData(stringData, stringDataSize));
    }

    std::unordered_map<juce::Identifier, juce::ValueTree> getAliases()
    {
        using namespace binary_data;

        std::unordered_map<juce::Identifier, juce::ValueTree> aliases{
            {
                "HomeIcon",
                parseLayout(home_svg, home_svgSize),
            },
            {
                "LayoutIcon",
                parseLayout(layout_svg, layout_svgSize),
            },
            {
                "PageLink",
                parseLayout(page_link_xml, page_link_xmlSize),
            },
            {
                "PaletteIcon",
                parseLayout(palette_svg, palette_svgSize),
            },
            {
                "Subtitle",
                parseLayout(subtitle_xml, subtitle_xmlSize),
            },
            {
                "Title",
                parseLayout(title_xml, title_xmlSize),
            },
        };

        aliases["PageLink"]
            .setProperty("style",
                         parseJSON(page_link_json, page_link_jsonSize),
                         nullptr);
        aliases["Subtitle"]
            .setProperty("style",
                         parseJSON(subtitle_json, subtitle_jsonSize),
                         nullptr);
        aliases["Title"]
            .setProperty("style",
                         parseJSON(title_json, title_jsonSize),
                         nullptr);

        return aliases;
    }
} // namespace jive_demo
