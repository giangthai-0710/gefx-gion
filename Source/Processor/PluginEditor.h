/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

#include "../GUI/SmallKnob.h"
#include "../GUI/MediumKnob.h"
#include "../GUI/LargeKnob.h"

//==============================================================================
/**
*/
class GiONAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    GiONAudioProcessorEditor (GiONAudioProcessor&);
    ~GiONAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    GiONAudioProcessor& audioProcessor;

    SmallKnob smallKnob;
    MediumKnob mediumKnob;
    LargeKnob largeKnob;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GiONAudioProcessorEditor)
};
