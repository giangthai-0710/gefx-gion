#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class MediumKnobLookAndFeel  : public juce::LookAndFeel_V4
{
public:
    MediumKnobLookAndFeel();

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                          float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override;

    void drawLabel(juce::Graphics& g, juce::Label& label) override;

private:
    juce::Image knobImage;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MediumKnobLookAndFeel)
};
