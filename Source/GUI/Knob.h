#pragma once

#include <JuceHeader.h>

#include "KnobLookAndFeel.h"

//==============================================================================
/*
*/
namespace GUI
{
    class Knob : public juce::Slider
    {
    public:
        Knob();
        ~Knob() override;

        void setKnobType(juce::String type);
        void setKnobSize(juce::String size);

        juce::String getTextFromValue(double value) override;

        void create(float minValue, float maxValue, float stepValue, float defaultValue, juce::String knobSize, juce::String knobType, juce::String knobDescription);

        void mouseEnter(const juce::MouseEvent& event) override;

        void mouseExit(const juce::MouseEvent& event) override;

        bool hasMouseOver();

    private:
        bool isMouseOver = false;

        KnobLookAndFeel knobLookAndFeel;
        juce::String knobSize;
        juce::String knobType;

        juce::String knobDescription;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Knob)
    };
}