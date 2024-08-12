#include "StageLED.h"

StageLED::StageLED()
{
}

StageLED::~StageLED()
{
}

void StageLED::paint(juce::Graphics& g)
{
	if (!isOn)
	{
		ledImage = juce::ImageCache::getFromMemory(BinaryData::LED_off_png, BinaryData::LED_off_pngSize);
	}
	else
	{
		if (distortionStage == 1)
		{
			ledImage = juce::ImageCache::getFromMemory(BinaryData::LED_on_png, BinaryData::LED_on_pngSize);
		}
		else
		{
			ledImage = juce::ImageCache::getFromMemory(BinaryData::LED_on_2_png, BinaryData::LED_on_2_pngSize);
		}
	}

	g.drawImage(ledImage, 0, 0, getWidth(), getHeight(), 0, 0, ledImage.getWidth(), ledImage.getHeight());
}

void StageLED::resized()
{
}

void StageLED::setIsOn(bool isOn)
{
	this->isOn = isOn;
	repaint();
}

void StageLED::setDistortionStage(int distortionStage)
{
	this->distortionStage = distortionStage;
	repaint();
}
