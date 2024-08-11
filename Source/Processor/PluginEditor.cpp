/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GiONAudioProcessorEditor::GiONAudioProcessorEditor (GiONAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

    addAndMakeVisible(smallKnob);
    addAndMakeVisible(mediumKnob);
    addAndMakeVisible(largeKnob);
}

GiONAudioProcessorEditor::~GiONAudioProcessorEditor()
{
    
}

//==============================================================================
void GiONAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::darkgrey);

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void GiONAudioProcessorEditor::resized()
{
    smallKnob.setBounds(0, 0, 48, 60);
    smallKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);

    mediumKnob.setBounds(48, 0, 64, 80);
    mediumKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);

    largeKnob.setBounds(112, 0, 96, 120);
    largeKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
}
