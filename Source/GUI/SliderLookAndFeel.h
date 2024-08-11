/*
  ==============================================================================

    SliderLookAndFeel.h
    Created: 11 Aug 2024 11:00:41pm
    Author:  giang

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class SliderLookAndFeel  : public juce::Component
{
public:
    SliderLookAndFeel();
    ~SliderLookAndFeel() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SliderLookAndFeel)
};
