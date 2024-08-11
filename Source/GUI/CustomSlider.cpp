/*
  ==============================================================================

    CustomSlider.cpp
    Created: 11 Aug 2024 11:00:28pm
    Author:  giang

  ==============================================================================
*/

#include <JuceHeader.h>
#include "CustomSlider.h"

//==============================================================================
CustomSlider::CustomSlider()
{
    setLookAndFeel(&sliderLookAndFeel);
}

CustomSlider::~CustomSlider()
{
    setLookAndFeel(nullptr);
}

void CustomSlider::mouseUp(const juce::MouseEvent& event)
{
    Slider::mouseUp(event);
    juce::Desktop::setMousePosition(mousePosition);
    setMouseCursor(juce::MouseCursor::NormalCursor);
}

void CustomSlider::mouseDown(const juce::MouseEvent& event)
{
    Slider::mouseDown(event);
	setMouseCursor(juce::MouseCursor::NoCursor);
    mousePosition = juce::Desktop::getMousePosition();
}


