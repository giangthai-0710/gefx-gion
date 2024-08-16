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
#include "../GUI/TooltipTextBox.h"

//==============================================================================
/**
*/
class GiONAudioProcessorComponent  : public juce::Component, juce::Timer
{
public:
    GiONAudioProcessorComponent (GiONAudioProcessor&);
    ~GiONAudioProcessorComponent() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    //==============================================================================
    void timerCallback() override;

    //==============================================================================
    void createKnobs();
    void createSwitches();

    void setTooltipText();

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    LookAndFeel::GeneralLNF generalLNF;
    
    GUI::TooltipTextBox tooltipTextBox;

    juce::Image background;

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
        
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GiONAudioProcessorComponent)
};


class ProcessorEditorWrapper : public juce::AudioProcessorEditor
{
public:
    ProcessorEditorWrapper(GiONAudioProcessor&);

    void resized() override;
private:
    static constexpr int originalWidth{ 480 };
    static constexpr int originalHeight{ 360 };
    GiONAudioProcessorComponent processorComponent;

    juce::ApplicationProperties applicationProperties;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProcessorEditorWrapper)

};