#pragma once

#include <JuceHeader.h>

#include "LargeKnobLookAndFeel.h"

//==============================================================================
/*
*/
class LargeKnob  : public juce::Slider
{
public:
    LargeKnob();
    ~LargeKnob() override;

private:
    LargeKnobLookAndFeel largeKnobLookAndFeel;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LargeKnob)
};
