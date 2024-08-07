/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
// Declare aliases 
using Filter = juce::dsp::IIR::Filter<float>;
using CutFilter = juce::dsp::ProcessorChain<Filter, Filter, Filter, Filter>;
using MonoChain = juce::dsp::ProcessorChain<CutFilter, Filter, CutFilter>;


class GiONAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    GiONAudioProcessor();
    ~GiONAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState apvts;

private:
    //==============================================================================
    // Parameters
    juce::AudioParameterFloat* stage1GainPtr, stage2GainPtr;
    juce::AudioParameterFloat* stage1CrunchPtr, stage2CrunchPtr;
    juce::AudioParameterFloat* stage1VolumePtr, stage2VolumeGainPtr;
    juce::AudioParameterFloat* stage1PreGainTonePtr, stage1PostGainTonePtr, stage2PreGainTonePtr, stage2PostGainTonePtr;
    juce::AudioParameterFloat* boostGainPtr, boostMidGainPtr, boostMidFrequencyPtr;

    juce::AudioParameterBool* stage1BypassPtr, stage2BypassPtr;
    juce::AudioParameterBool* boostBypassPtr;
    juce::AudioParameterBool* boostMidBypassPtr;

    juce::AudioParameterChoice* boostLocationChoicePtr;

    //==============================================================================
    // Filters
    MonoChain leftChain, rightChain;
    Filter stage1PreGainToneFilter, stage1PostGainToneFilter, stage2PreGainToneFilter, stage2PostGainToneFilter;
    Filter boostMidFilter;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GiONAudioProcessor)
};
