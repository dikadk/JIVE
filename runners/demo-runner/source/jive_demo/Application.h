#pragma once

#include <jive_demo/gui/WindowPresenter.h>

namespace jive_demo
{
    class Application : public juce::JUCEApplication
    {
    public:
        Application() = default;

        const juce::String getApplicationName() final
        {
            return JIVE_DEMO_APPLICATION_NAME;
        }

        const juce::String getApplicationVersion() final
        {
            return JIVE_DEMO_APPLICATION_VERSION;
        }

        void initialise(const juce::String& /*commandLineArguments*/) final
        {
            window = windowPresenter.present();
        }

        void shutdown() final
        {
            window = nullptr;
        }

    private:
        WindowPresenter windowPresenter;
        std::unique_ptr<jive::GuiItem> window;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Application)
    };
} // namespace jive_demo
