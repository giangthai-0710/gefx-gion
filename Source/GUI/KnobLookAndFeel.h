#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class KnobLookAndFeel  : public juce::LookAndFeel_V4
{
public:
    KnobLookAndFeel();
   
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                          float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override;


    void drawLabel(juce::Graphics& g, juce::Label& label) override;

    void drawBubble(juce::Graphics& g, juce::BubbleComponent& comp, const juce::Point<float>& tip, const juce::Rectangle<float>& body) override;

    void setKnobSize(juce::String size);

    //juce::Font getSliderPopupFont(juce::Slider&) override;
    


private:
    juce::Image knobImage;
    juce::String knobSize;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KnobLookAndFeel)
};
