#pragma once

namespace jive
{
    class Length : public Property<juce::String>
    {
    public:
        using Property<juce::String>::Property;

        float toPixels(const juce::Rectangle<float>& parentBounds) const;

        bool isPixels() const;
        bool isPercent() const;

        using Property<juce::String>::operator=;

        static const float pixelValueWhenAuto;

    private:
        double getRelativeParentLength(const juce::Rectangle<double>& parentBounds) const;
    };
} // namespace jive
