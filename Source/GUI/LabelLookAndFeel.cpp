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
		auto alpha = label.isEnabled() ? 1.0f : 0.5f;
		const auto font = juce::Font("Arial", fontSize, juce::Font::bold);

		g.setColour(label.findColour(juce::Label::textColourId).withMultipliedAlpha(alpha));
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
		fontSize = 10;
	}
	else
	{
		fontSize = 15;
	}
}

