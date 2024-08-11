#pragma once

#include <JuceHeader.h>

#include "MediumKnobLookAndFeel.h"

class MediumKnob  : public juce::Slider
{
public:
    MediumKnob();
    ~MediumKnob() override;

private:
    MediumKnobLookAndFeel mediumKnobLookAndFeel;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MediumKnob)
};
