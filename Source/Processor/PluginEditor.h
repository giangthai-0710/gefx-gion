/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

#include "../GUI/Knob.h"
#include "../GUI/Label.h"
#include "../GUI/Switch.h"
#include "../GUI/StageLED.h"
#include "../GUI/GainLED.h"

#include "../GUI/GeneralLookAndFeel.h"
#include "../GUI/PresetPanel.h"

//==============================================================================
/**
*/
class GiONAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Timer
{
public:
    GiONAudioProcessorEditor (GiONAudioProcessor&);
    ~GiONAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void timerCallback() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    LookAndFeel::GeneralLNF generalLNF;


    juce::Image background;

    juce::TooltipWindow tooltipWindow;

    GiONAudioProcessor& audioProcessor;

    GUI::Knob gainKnob, crunchKnob, volumeKnob,
         preBassKnob, preMidKnob, preTrebleKnob, 
         postBassKnob, postMidKnob, postTrebleKnob;

    GUI::Label gainLabel, crunchLabel, volumeLabel,
          preBassLabel, preMidLabel, preTrebleLabel, 
          postBassLabel, postMidLabel, postTrebleLabel,
          bypassLabel, changeStageLabel;

    GUI::Switch bypassSwitch, changeStageSwitch;

    GUI::PresetPanel presetPanel;

    StageLED stageLED;
    GainLED gainLED;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment, crunchAttachment, volumeAttachment,
                                                                          preBassAttachment, preMidAttachment, preTrebleAttachment,
                                                                          postBassAttachment, postMidAttachment, postTrebleAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> bypassAttachment, changeStageAttachment;
        
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GiONAudioProcessorEditor)
};
