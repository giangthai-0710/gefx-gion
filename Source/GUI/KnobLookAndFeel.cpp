#include <JuceHeader.h>
#include "KnobLookAndFeel.h"

//==============================================================================
KnobLookAndFeel::KnobLookAndFeel()
{
    setKnobSize("large");
    setDefaultSansSerifTypefaceName("Now");    
}

void KnobLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
{
    if (knobImage.isValid())
    {
        const double fractRotation = (slider.getValue() - slider.getMinimum()) / (slider.getMaximum() - slider.getMinimum());

        const int nFrames = 128;
        const int frameId = (int)ceil(fractRotation * ((double)nFrames - 1));

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

void KnobLookAndFeel::drawLabel(juce::Graphics& g, juce::Label& label)
{
    g.fillAll(juce::Colour(0xff202020)); // Background color of the label
    g.setColour(juce::Colours::white); // Text color
    g.setFont(12.0f); // Font size
    g.drawText(label.getText(), label.getLocalBounds(), juce::Justification::centred);
}

void KnobLookAndFeel::drawBubble(juce::Graphics& g, juce::BubbleComponent& comp, const juce::Point<float>& tip, const juce::Rectangle<float>& body)
{
    
    juce::Path p;
    p.addBubble(body.reduced(0.5f), body.getUnion(juce::Rectangle<float>(tip.x, tip.y, 1.0f, 1.0f)),
                tip, 5.0f, juce::jmin(15.0f, body.getWidth() * 0.2f, body.getHeight() * 0.2f));

    g.setColour(juce::Colour(0xff303030));
    g.fillPath(p);

    g.setColour(juce::Colours::white);
    g.strokePath(p, juce::PathStrokeType(1.0f));
}

void KnobLookAndFeel::setKnobSize(juce::String size)
{
    if (size == "large")
	{
		knobImage = juce::ImageCache::getFromMemory(BinaryData::Knob_large_png, BinaryData::Knob_large_pngSize);
	}
	else if (size == "medium")
	{
		knobImage = juce::ImageCache::getFromMemory(BinaryData::Knob_medium_png, BinaryData::Knob_medium_pngSize);
	}
	else if (size == "small")
	{
		knobImage = juce::ImageCache::getFromMemory(BinaryData::Knob_small_png, BinaryData::Knob_small_pngSize);
	}
    else
    {
        knobImage = juce::ImageCache::getFromMemory(BinaryData::Knob_large_png, BinaryData::Knob_large_pngSize);
    }
}


