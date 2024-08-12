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
    float distortionGain{ 0 }, distortionCrunch{ 0 }, distortionVolume{ 0 },
          preBassGain{ 0 }, preTrebleGain { 0 },
          postBassGain{ 0 }, postTrebleGain{ 0 },
          midBoostGain { 0 };

    bool distortionBypass{ false }, distortionStage{ false };
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

    FilterChain preLeftFilter, preRightFilter,
                postLeftFilter, postRightFilter;

    Filter midBoostLeftFilter, midBoostRightFilter;

    Coefficients preBassCoefficients, preTrebleCoefficients,
				 postBassCoefficients, postTrebleCoefficients,
				 midBoostCoefficients;

    enum FilterType
    {
        bassFilter,
        trebleFilter
    };

    float preBassFrequency = 120.0f, preTrebleFrequency = 3000.0f,
          postBassFrequency = 120.0f, postTrebleFrequency = 3000.0f,
          midBoostFrequency = 900.0f;

    float preBassQ = 0.707f, preTrebleQ = 0.707f,
		  postBassQ = 0.707f, postTrebleQ = 0.707f,
		  midBoostQ = 0.707f;

    void updateCoefficients(Coefficients& old, Coefficients& replacement)
    {
        *old = *replacement;
    }



    //==============================================================================
    // Distortion
    DistortionDSP distortionStage;

    juce::dsp::StateVariableTPTFilter<float> antiAliasingFilter, 
                                             preGainHighPassFilter, 
                                             postGainLowPassFilter;
    juce::dsp::ProcessSpec processSpec;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GiONAudioProcessor)
};
