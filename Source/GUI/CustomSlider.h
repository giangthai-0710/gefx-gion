/*
  ==============================================================================

    CustomSlider.h
    Created: 11 Aug 2024 11:00:28pm
    Author:  giang

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class CustomSlider  : public juce::Component
{
public:
    CustomSlider();
    ~CustomSlider() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomSlider)
};
