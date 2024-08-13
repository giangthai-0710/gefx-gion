#pragma once

#include <JuceHeader.h>

namespace LookAndFeel
{
	class GeneralLNF : public juce::LookAndFeel_V4
	{
	public:
		GeneralLNF();
		~GeneralLNF();

	private:
		// For texts
		juce::Font defaultFont;
		juce::Colour defaultFontColour;

		juce::Colour defaultBackgroundColour;


		juce::Colour defaultTooltipOutlineColour;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GeneralLNF)
	};
}