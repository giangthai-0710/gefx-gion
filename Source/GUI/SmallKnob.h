#pragma once

#include <JuceHeader.h>

#include "SmallKnobLookAndFeel.h"

//==============================================================================
/*
*/
class SmallKnob  : public juce::Slider
{
public:
    SmallKnob();
    ~SmallKnob() override;

private:
    SmallKnobLookAndFeel smallKnobLookAndFeel;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SmallKnob)
};
