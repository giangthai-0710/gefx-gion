#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class GainLED  : public juce::Component
{
public:
    GainLED();
    ~GainLED() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void setGainPercentage(float newGainPercentage);

    void setIsOn(bool isOn);

private:
    juce::Image ledImage;
    bool isOn = false;

    float gainPercentage;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GainLED)
};
