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
using Coefficients = Filter::CoefficientsPtr;

//==============================================================================
struct parametersStruct
{
    float stage1Gain { 0 }, stage2Gain { 0 },
          stage1Crunch { 0 }, stage2Crunch { 0 }, 
          stage1Volume { 0 }, stage2Volume { 0 }, 
          preStageBassGain{ 0 }, preStageTrebleGain { 0 },
          postStage1BassGain{ 0 }, postStage1TrebleGain{ 0 },
          postStage2BassGain{ 0 }, postStage2TrebleGain{ 0 },
          midBoostGain { 0 };

    bool stage1Bypass{ false }, stage2Bypass{ false };
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
    void updateFilters();

private:
    //==============================================================================
    // Filters
    using FilterChain = juce::dsp::ProcessorChain<Filter, Filter>;

    FilterChain preStageLeftFilter, preStageRightFilter,
				postStage1LeftFilter, postStage1RightFilter,
				postStage2LeftFilter, postStage2RightFilter;

    Filter midBoostLeftFilter, midBoostRightFilter;

    Coefficients preStageBassCoefficients, preStageTrebleCoefficients,
				 postStage1BassCoefficients, postStage1TrebleCoefficients,
				 postStage2BassCoefficients, postStage2TrebleCoefficients,
				 midBoostCoefficients;

    enum FilterType
    {
        bassFilter,
        trebleFilter
    };

    float preStageBassFrequency = 120.0f, preStageTrebleFrequency = 3000.0f,
        postStage1BassFrequency = 120.0f, postStage1TrebleFrequency = 3000.0f,
        postStage2BassFrequency = 120.0f, postStage2TrebleFrequency = 3000.0f,
        midBoostFrequency = 900.0f;

    float preStageBassQ = 0.707f, preStageTrebleQ = 0.707f,
		  postStage1BassQ = 0.707f, postStage1TrebleQ = 0.707f,
		  postStage2BassQ = 0.707f, postStage2TrebleQ = 0.707f,
		  midBoostQ = 0.707f;

    void updateCoefficients(Coefficients& old, Coefficients& replacement)
    {
        *old = *replacement;
    }



    //==============================================================================
    // Distortion
    DistortionDSP stage1Distortion, stage2Distortion;

    juce::dsp::StateVariableTPTFilter<float> antiAliasingFilter, 
                                             preGainHighPassFilter, 
                                             postGainLowPassFilter;
    juce::dsp::ProcessSpec processSpec;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GiONAudioProcessor)
};
