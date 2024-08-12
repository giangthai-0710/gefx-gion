/*
  ==============================================================================

    GainLED.cpp
    Created: 12 Aug 2024 9:27:08pm
    Author:  giang

  ==============================================================================
*/

#include <JuceHeader.h>
#include "GainLED.h"

//==============================================================================
GainLED::GainLED()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

GainLED::~GainLED()
{
}

void GainLED::paint (juce::Graphics& g)
{
    if (isOn)
    {
        ledImage = juce::ImageCache::getFromMemory(BinaryData::LED_changeable_png, BinaryData::LED_changeable_pngSize);

        const int nFrames = 32;
        const int frameWidth = ledImage.getWidth() / nFrames;
        const int frameHeight = ledImage.getHeight();

        int frameId = (int)ceil(gainPercentage * (float)(nFrames - 1));

        g.drawImage(ledImage, 0, 0, frameWidth, frameHeight, frameId * frameWidth, 0, frameWidth, frameHeight);
    }
    else
    {
        ledImage = juce::ImageCache::getFromMemory(BinaryData::LED_off_png, BinaryData::LED_off_pngSize);
		g.drawImage(ledImage, 0, 0, getWidth(), getHeight(), 0, 0, ledImage.getWidth(), ledImage.getHeight());
    }

}

void GainLED::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void GainLED::setGainPercentage(float newGainPercentage)
{
    this->gainPercentage = newGainPercentage;
    repaint();
}

void GainLED::setIsOn(bool isOn)
{
    this->isOn = isOn;
    repaint();
}
