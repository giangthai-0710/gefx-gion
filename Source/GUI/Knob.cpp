#include <JuceHeader.h>
#include "Knob.h"

//==============================================================================
GUI::Knob::Knob()
{
	setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    setLookAndFeel(&knobLookAndFeel);
    setPopupDisplayEnabled(true, false , nullptr, -1);
	setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
	
	knobType = "null";

	addMouseListener(this, true);
}

GUI::Knob::~Knob()
{
    setLookAndFeel(nullptr);
}

void GUI::Knob::setKnobType(juce::String type)
{
    this->knobType = type;
}

void GUI::Knob::setKnobSize(juce::String size)
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

juce::String GUI::Knob::getTextFromValue(double value)
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

void GUI::Knob::create(float minValue, float maxValue, float stepValue, float defaultValue, juce::String knobSize, juce::String knobType, juce::String knobDescription)
{
	setRange(minValue, maxValue, stepValue);
	setValue(defaultValue);
	setKnobSize(knobSize);
	setKnobType(knobType);
	setDescription(knobDescription);

}

void GUI::Knob::mouseEnter(const juce::MouseEvent& event)
{
	isMouseOver = true;
}

void GUI::Knob::mouseExit(const juce::MouseEvent& event)
{
	isMouseOver = false;
}

bool GUI::Knob::hasMouseOver()
{
	return isMouseOver;
}




