/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DistortionDSP.h"

//==============================================================================
// Declare aliases 
using Filter = juce::dsp::IIR::Filter<float>;
using CutFilter = juce::dsp::ProcessorChain<Filter, Filter, Filter, Filter>;
using MonoChain = juce::dsp::ProcessorChain<CutFilter, Filter, CutFilter>;
using Coefficients = Filter::CoefficientsPtr;

//==============================================================================
struct parametersStruct
{
    float stage1Gain { 0 }, stage2Gain { 0 },
          stage1Crunch { 0 }, stage2Crunch { 0 }, 
          stage1Volume { 0 }, stage2Volume { 0 }, 
          stage1PreGainTone { 0 }, stage2PreGainTone { 0 }, 
          stage1PostGainTone { 0 }, stage2PostGainTone { 0 }, 
          boostMidGain { 0 };

    bool stage1Bypass { false }, stage2Bypass { false };
};

parametersStruct getParametersFromTree(juce::AudioProcessorValueTreeState& apvts);

class GiONAudioProcessor : public juce::AudioProcessor
{
public:
    //==============================================================================
    GiONAudioProcessor();
    ~GiONAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

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
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState apvts{ *this, nullptr, "Parameters", createParameterLayout() };
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    void updateParameters();

private:
    //==============================================================================
    // Parameters
    //juce::AudioParameterFloat* stage1GainPtr, stage2GainPtr;
    //juce::AudioParameterFloat* stage1CrunchPtr, stage2CrunchPtr;
    //juce::AudioParameterFloat* stage1VolumePtr, stage2VolumePtr;
    //juce::AudioParameterFloat* stage1PreGainTonePtr, stage2PreGainTonePtr;
    //juce::AudioParameterFloat* stage1PostGainTonePtr, stage2PostGainTonePtr;
    //juce::AudioParameterFloat* boostMidGainPtr;

    //==============================================================================
    // Filters
    MonoChain leftAntiAliasingChain, rightAntiAliasingChain;
    Filter stage1PreGainToneFilter, stage1PostGainToneFilter, stage2PreGainToneFilter, stage2PostGainToneFilter;
    Filter boostMidFilter;

    //==============================================================================
    // Distortion
    DistortionDSP stage1Distortion, stage2Distortion;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GiONAudioProcessor)
};
