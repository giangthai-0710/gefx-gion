#include "GeneralLookAndFeel.h"

LookAndFeel::GeneralLNF::GeneralLNF()
{
	defaultFont = juce::Font("Now", 12.0f, juce::Font::plain);
	defaultFontColour = juce::Colours::white;

    defaultBackgroundColour = juce::Colours::black;

    defaultTooltipOutlineColour = juce::Colours::white;

	setColour(juce::PopupMenu::backgroundColourId, juce::Colour(0xff1A1A1A));
}

LookAndFeel::GeneralLNF::~GeneralLNF()
{
}

