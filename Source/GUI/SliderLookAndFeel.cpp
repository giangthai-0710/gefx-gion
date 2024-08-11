/*
  ==============================================================================

    SliderLookAndFeel.cpp
    Created: 11 Aug 2024 11:00:41pm
    Author:  giang

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SliderLookAndFeel.h"

//==============================================================================
SliderLookAndFeel::SliderLookAndFeel()
{
    knobImage = juce::ImageCache::getFromMemory (BinaryData::Knob_small_png, BinaryData::Knob_small_pngSize);
}

void SliderLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
{
    if (knobImage.isValid())
    {
        const double fractRotation = (slider.getValue() - slider.getMinimum()) / (slider.getMaximum() - slider.getMinimum());

        const int nFrames = 128;
        const int frameId = (int)ceil(fractRotation * ((double)nFrames - 1));
        const float radius = juce::jmin(width / 2, height / 2) - 4.0f;
        const float centerX = x + width * 0.5f;
        const float centerY = y + height * 0.5f;
        const float rx = centerX - radius;
        const float ry = centerY - radius;

        int imgWidth = knobImage.getWidth() / nFrames;
        int imgHeight = knobImage.getHeight();

        g.drawImage(knobImage, 0, 0, imgWidth, imgHeight, frameId * imgWidth, 0, imgWidth, imgHeight);
    }
    else
    {
        static const float textPpercent = 0.35f;
        juce::Rectangle<float> text_bounds(1.0f + width * (1.0f - textPpercent) / 2.0f, 0.5f * height, width * textPpercent, 0.5f * height);

        g.setColour(juce::Colours::white);

        g.drawFittedText(juce::String("No Image"), text_bounds.getSmallestIntegerContainer(), juce::Justification::horizontallyCentred | juce::Justification::centred, 1);

    }
}

void SliderLookAndFeel::drawLabel(juce::Graphics& g, juce::Label& label)
{

}




