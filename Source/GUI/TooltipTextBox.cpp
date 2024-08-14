#include "TooltipTextBox.h"

GUI::TooltipTextBox::TooltipTextBox()
{
	defaultFont = juce::Font("Now", 15.0f, juce::Font::bold);

	textToDisplay = "";
}

void GUI::TooltipTextBox::paint(juce::Graphics& g)
{
	g.setColour(juce::Colour(0xffb8b8b8));
	g.setFont(defaultFont);
	g.drawText(textToDisplay, getLocalBounds(), juce::Justification::centredLeft, true);
}

void GUI::TooltipTextBox::displayKnobDescription(GUI::Knob& knob)
{
	if (knob.hasMouseOver())
	{
		textToDisplay = knob.getDescription();
		DBG(knob.getDescription());
		repaint();
	}
}

void GUI::TooltipTextBox::displaySwitchDescription(GUI::Switch& sw)
{
	textToDisplay = sw.getDescription();
	DBG(textToDisplay);
	repaint();
}

void GUI::TooltipTextBox::clear()
{
	textToDisplay = "";
	repaint();
}

