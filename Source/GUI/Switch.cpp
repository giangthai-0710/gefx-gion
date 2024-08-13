#include <JuceHeader.h>
#include "Switch.h"

//==============================================================================
GUI::Switch::Switch()
{
    switchImage = juce::ImageCache::getFromMemory(BinaryData::Switch_png, BinaryData::Switch_pngSize);
    switchPressedImage = juce::ImageCache::getFromMemory(BinaryData::Switch_pressed_png, BinaryData::Switch_pressed_pngSize);

    this->onClick = [this] 
        { 
            startTimer(100); // Start a timer to reset the button to be unpressed after 100ms
            isPressed = true; // Set the button to be pressed
            repaint(); // Repaint the button
        }; // When the button is clicked, set the button to be pressed
}

GUI::Switch::~Switch()
{
}

void GUI::Switch::paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    if (!isPressed)
    {
        g.drawImage(switchImage, 0, 0, getWidth(), getHeight(), 0, 0, switchImage.getWidth(), switchImage.getHeight());
	}
	else
	{
		g.drawImage(switchPressedImage, 0, 0, getWidth(), getHeight(), 0, 0, switchPressedImage.getWidth(), switchPressedImage.getHeight());
    }
}

