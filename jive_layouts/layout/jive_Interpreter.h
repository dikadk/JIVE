#pragma once

namespace jive
{
    class Interpreter
    {
    public:
        const ComponentFactory& getComponentFactory() const;
        ComponentFactory& getComponentFactory();
        void setComponentFactory(const ComponentFactory& newFactory);

        void setAlias(juce::Identifier aliasType, const juce::ValueTree& treeToReplaceWith);

        template <typename Decorator>
        void addDecorator(const juce::Identifier& itemType);

        [[nodiscard]] std::unique_ptr<GuiItem> interpret(const juce::ValueTree& tree) const;
        [[nodiscard]] std::unique_ptr<GuiItem> interpret(const juce::XmlElement& xml) const;
        [[nodiscard]] std::unique_ptr<GuiItem> interpret(const juce::String& xmlString) const;
        [[nodiscard]] std::unique_ptr<GuiItem> interpret(const void* xmlStringData, int xmlStringDataSize) const;

    private:
        std::unique_ptr<GuiItem> interpret(const juce::ValueTree& tree, GuiItem* const parent) const;

        void expandAlias(juce::ValueTree& tree) const;

        std::unique_ptr<GuiItem> createUndecoratedItem(const juce::ValueTree& tree, GuiItem* const parent) const;
        void appendChild(GuiItem& item, const juce::ValueTree& childState) const;
        void appendChildItems(GuiItem& item) const;

        std::unique_ptr<juce::Component> createComponent(const juce::ValueTree& tree) const;

        ComponentFactory componentFactory;
        std::vector<std::pair<juce::Identifier, std::function<std::unique_ptr<GuiItemDecorator>(std::unique_ptr<GuiItem>)>>> customDecorators;
        std::unordered_map<juce::Identifier, juce::ValueTree> aliases;
        juce::Array<juce::Identifier> typesWithoutStyleSheets{
            "Text",
            "Image",
        };

        JUCE_LEAK_DETECTOR(Interpreter)
    };
} // namespace jive
