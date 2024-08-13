/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DistortionDSP.h"

#include "../Service/PresetManager.h"

//==============================================================================
// Declare aliases 
using Filter = juce::dsp::IIR::Filter<float>;
using Coefficients = Filter::CoefficientsPtr;

//==============================================================================
struct parametersStruct
{
    float distortionGain{ 0 }, distortionCrunch{ 0 }, distortionVolume{ 0 },
        preBassGain{ 0 }, preMidGain{ 0 }, preTrebleGain{ 0 },
        postBassGain{ 0 }, postMidGain{ 0 }, postTrebleGain{ 0 };

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

    float getRMSLevel();

    Service::PresetManager& getPresetManager() { return *presetManager; }

private:
    //==============================================================================
    // Filters
    using FilterChain = juce::dsp::ProcessorChain<Filter, Filter ,Filter>;

    FilterChain preLeftFilter, preRightFilter,
                postLeftFilter, postRightFilter;

    Coefficients preBassCoefficients, preMidCoefficients, preTrebleCoefficients,
                 postBassCoefficients, postMidCoefficients, postTrebleCoefficients;

    enum FilterType
    {
        bassFilter,
        midFilter,
        trebleFilter
    };

    float bassFrequency = 120.0f, midFrequency = 800.0f, trebleFrequency = 4000.0f;

    float bassQ = 0.707f, midQ = 0.707f, trebleQ = 0.707f;

    void updateCoefficients(Coefficients& old, Coefficients& replacement)
    {
        *old = *replacement;
    }

    //==============================================================================
    // Distortion
    DSP::DistortionDSP distortionStage;

    float distortionLevelLeft, distortionLevelRight, distortionLevel;

    juce::dsp::StateVariableTPTFilter<float> antiAliasingFilter, 
                                             preGainHighPassFilter, 
                                             postGainLowPassFilter;
    juce::dsp::ProcessSpec processSpec;

    //==============================================================================
    std::unique_ptr<Service::PresetManager> presetManager;
 

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GiONAudioProcessor)
};
