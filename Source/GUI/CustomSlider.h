#pragma once

#include <JuceHeader.h>
#include "SliderLookAndFeel.h"

//==============================================================================
/*
*/
class CustomSlider  : public juce::Slider
{
public:
    CustomSlider();
    ~CustomSlider() override;

    void mouseUp(const juce::MouseEvent& event) override;
    void mouseDown(const juce::MouseEvent& event) override;

private:
    SliderLookAndFeel sliderLookAndFeel;
    juce::Point<int> mousePosition;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomSlider)
};
