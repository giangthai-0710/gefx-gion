#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class SwitchLookAndFeel  : public juce::LookAndFeel_V4
{
public:
    SwitchLookAndFeel();
    ~SwitchLookAndFeel() override;

    void drawToggleButton (juce::Graphics& g, juce::ToggleButton& button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

    void setRecentlyPressed(bool isRecentlyPressed);

private:
    juce::Image switchImage, switchPressedImage;
    bool isRecentlyPressed = false;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SwitchLookAndFeel)
};
