#pragma once

#include <JuceHeader.h>

#include "KnobLookAndFeel.h"

//==============================================================================
/*
*/
class Knob  : public juce::Slider
{
public:
    Knob();
    ~Knob() override;

    void setKnobType(juce::String type);
    void setKnobSize(juce::String size);

    juce::String getTextFromValue (double value) override;



private:
    KnobLookAndFeel knobLookAndFeel;
    juce::String knobSize;
    juce::String knobType;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Knob)
};
