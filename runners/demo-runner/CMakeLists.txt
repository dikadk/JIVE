juce_add_gui_app(jive-demo-runner
    PRODUCT_NAME "JIVE Demo Runner"
    VERSION 0.1.0
)

target_sources(jive-demo-runner
PRIVATE
    source/jive_demo/Application.h
    source/jive_demo/gui/aliases/Aliases.h
    source/jive_demo/gui/Layouts.h
    source/jive_demo/gui/pages/HomePagePresenter.h
    source/jive_demo/gui/WindowPresenter.h
    source/main.cpp
)

juce_add_binary_data(jive-demo-runner-resources
    HEADER_NAME "binary_data.h"
    NAMESPACE "binary_data"

SOURCES
    source/jive_demo/gui/aliases/icons/home.svg
    source/jive_demo/gui/aliases/icons/layout.svg
    source/jive_demo/gui/aliases/icons/palette.svg
    source/jive_demo/gui/aliases/page-link/page_link.json
    source/jive_demo/gui/aliases/page-link/page_link.xml
    source/jive_demo/gui/aliases/subtitle/subtitle.json
    source/jive_demo/gui/aliases/subtitle/subtitle.xml
    source/jive_demo/gui/aliases/title/title.json
    source/jive_demo/gui/aliases/title/title.xml
    source/jive_demo/gui/pages/home_page.xml
    source/jive_demo/gui/style.json
    source/jive_demo/gui/window.xml
)

target_compile_definitions(jive-demo-runner
PRIVATE
    JIVE_DEMO_APPLICATION_NAME="$<TARGET_PROPERTY:jive-demo-runner,JUCE_PRODUCT_NAME>"
    JIVE_DEMO_APPLICATION_VERSION="$<TARGET_PROPERTY:jive-demo-runner,JUCE_VERSION>"
    JIVE_GUI_ITEMS_HAVE_STYLE_SHEETS=1
)

target_link_libraries(jive-demo-runner
PRIVATE
    jive::jive_layouts
    jive::jive_style_sheets
    jive-demo-runner-resources
)

target_include_directories(jive-demo-runner
PRIVATE
    source
)

target_compile_features(jive-demo-runner
PRIVATE
    cxx_std_17
)

target_compile_options(jive-demo-runner
PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>

    -fsanitize=address
    -fsanitize=undefined
)

target_link_options(jive-demo-runner
PRIVATE
    -fsanitize=address
    -fsanitize=undefined
)
