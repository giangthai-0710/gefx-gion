#pragma once

#include <JuceHeader.h>

#include "Switch.h"
#include "Knob.h"

namespace GUI
{
	class TooltipTextBox : public juce::Component
	{
	public:
		TooltipTextBox();

		void paint(juce::Graphics& g) override;

		void displayKnobDescription(GUI::Knob& knob);

		void displaySwitchDescription(GUI::Switch& sw);

		void clear();

	private:
		juce::Font defaultFont;

		juce::String textToDisplay;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TooltipTextBox)
	};
}