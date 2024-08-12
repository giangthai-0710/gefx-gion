#include <JuceHeader.h>
#include "SwitchLookAndFeel.h"

//==============================================================================
SwitchLookAndFeel::SwitchLookAndFeel()
{
    switchImage = juce::ImageCache::getFromMemory(BinaryData::Switch_png, BinaryData::Switch_pngSize);
    switchPressedImage = juce::ImageCache::getFromMemory(BinaryData::Switch_pressed_png, BinaryData::Switch_pressed_pngSize);
}

SwitchLookAndFeel::~SwitchLookAndFeel()
{
}

void SwitchLookAndFeel::drawToggleButton(juce::Graphics& g, juce::ToggleButton& button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    if (!isRecentlyPressed)
    {
        g.drawImage(switchImage, 0, 0, button.getWidth(), button.getHeight(), 0, 0, switchImage.getWidth(), switchImage.getHeight());
    }
    else
    {
		g.drawImage(switchPressedImage, 0, 0, button.getWidth(), button.getHeight(), 0, 0, switchPressedImage.getWidth(), switchPressedImage.getHeight());
	} 
}

void SwitchLookAndFeel::setRecentlyPressed(bool isRecentlyPressed)
{
	this->isRecentlyPressed = isRecentlyPressed;
}



