#include <JuceHeader.h>
#include "Knob.h"

//==============================================================================
Knob::Knob()
{
	setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    setLookAndFeel(&knobLookAndFeel);
    setPopupDisplayEnabled(true, false , nullptr, -1);
	setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
	knobType = "null";
}

Knob::~Knob()
{
    setLookAndFeel(nullptr);
}

void Knob::setKnobType(juce::String type)
{
    this->knobType = type;
}

void Knob::setKnobSize(juce::String size)
{
	if (size == "large")
	{
		knobLookAndFeel.setKnobSize("large");
		setLookAndFeel(&knobLookAndFeel);
	}
	else if (size == "small")
	{
		knobLookAndFeel.setKnobSize("small");
		setLookAndFeel(&knobLookAndFeel);
	}
	else if (size == "medium")
	{
		knobLookAndFeel.setKnobSize("medium");
		setLookAndFeel(&knobLookAndFeel);
	}
}

juce::String Knob::getTextFromValue(double value)
{
    if (knobType == "volume")
    {
        return juce::String(value, 2) + " dB";
    }
    else if (knobType == "frequency")
	{
		return juce::String(value, 2) + " Hz";
	}
	else if (knobType == "time")
	{
		return juce::String(value, 2) + " ms";
	}
	else if (knobType == "percent")
	{
		return juce::String(value, 2) + " %";
	}
	else
	{
		return juce::String(value, 2);
	}
}



