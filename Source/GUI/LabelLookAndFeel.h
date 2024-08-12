#pragma once

#include <JuceHeader.h>

class LabelLookAndFeel : public juce::LookAndFeel_V4
{
public:
    LabelLookAndFeel();
    ~LabelLookAndFeel() override;

    void drawLabel (juce::Graphics& g, juce::Label& label) override;

    void setLabelSize (juce::String size);

private:
    int fontSize;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LabelLookAndFeel)
};
