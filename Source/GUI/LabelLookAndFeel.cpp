#include <JuceHeader.h>
#include "LabelLookAndFeel.h"

//==============================================================================
LabelLookAndFeel::LabelLookAndFeel()
{
    
}

LabelLookAndFeel::~LabelLookAndFeel()
{
}

void LabelLookAndFeel::drawLabel(juce::Graphics& g, juce::Label& label)
{
	g.fillAll(label.findColour(juce::Label::backgroundColourId));

	if (!label.isBeingEdited())
	{
		const auto font = juce::Font("Now", fontSize, juce::Font::bold);

		g.setColour(juce::Colour(0xffb1b1b1));
		g.setFont(font);

		auto textArea = label.getBorderSize().subtractedFrom(label.getLocalBounds());

		g.drawFittedText(label.getText(), textArea, label.getJustificationType(),
			juce::jmax(1, (int)(textArea.getHeight() / font.getHeight())),
			label.getMinimumHorizontalScale());
	}
	else if (label.isEnabled())
	{
		g.setColour(label.findColour(juce::Label::outlineColourId));
		g.drawRect(label.getLocalBounds());
	}
}

void LabelLookAndFeel::setLabelSize(juce::String size)
{
	if (size == "large")
	{
		fontSize = 15;
	}
	else if (size == "medium")
	{
		fontSize = 12;
	}
	else if (size == "small")
	{
		fontSize = 7;
	}
	else
	{
		fontSize = 15;
	}
}

