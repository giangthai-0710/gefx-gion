/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GiONAudioProcessor::GiONAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif 
{
    antiAliasingFilter.setCutoffFrequency(12000.0f);
    antiAliasingFilter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
    antiAliasingFilter.setResonance(0.7071f);

    preGainHighPassFilter.setCutoffFrequency(25.0f);
    preGainHighPassFilter.setType(juce::dsp::StateVariableTPTFilterType::highpass);
    preGainHighPassFilter.setResonance(0.7071f);

    postGainLowPassFilter.setCutoffFrequency(8000.0f);
    postGainLowPassFilter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
    postGainLowPassFilter.setResonance(0.7071f);
}

GiONAudioProcessor::~GiONAudioProcessor()
{
}

//==============================================================================
const juce::String GiONAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool GiONAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool GiONAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool GiONAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double GiONAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int GiONAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int GiONAudioProcessor::getCurrentProgram()
{
    return 0;
}

void GiONAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String GiONAudioProcessor::getProgramName (int index)
{
    return {};
}

void GiONAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void GiONAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    processSpec.sampleRate = sampleRate;
    processSpec.maximumBlockSize = samplesPerBlock;
    processSpec.numChannels = getTotalNumOutputChannels();

    antiAliasingFilter.prepare(processSpec);
    antiAliasingFilter.reset();

    preGainHighPassFilter.prepare(processSpec);
    preGainHighPassFilter.reset();

    postGainLowPassFilter.prepare(processSpec);
    postGainLowPassFilter.reset();

    preStageLeftFilter.prepare(processSpec);
    preStageRightFilter.prepare(processSpec);
    preStageLeftFilter.reset();
    preStageRightFilter.reset();

    postStage1LeftFilter.prepare(processSpec);
    postStage1RightFilter.prepare(processSpec);
    postStage1LeftFilter.reset();
    postStage1RightFilter.reset();

    postStage2LeftFilter.prepare(processSpec);
    postStage2RightFilter.prepare(processSpec);
    postStage2LeftFilter.reset();
    postStage2RightFilter.reset();

    midBoostLeftFilter.prepare(processSpec);
    midBoostRightFilter.prepare(processSpec);
    midBoostLeftFilter.reset();
}

void GiONAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool GiONAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void GiONAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    // In case we have more output channels than input channels, we'll clear them
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    parametersStruct parameters = getParametersFromTree(apvts);
    updateParameters();
    updateFilters();

    // Context for the audio block
    auto myBlock = juce::dsp::AudioBlock<float>(buffer);
    auto context = juce::dsp::ProcessContextReplacing<float>(myBlock);

    auto leftBlock = myBlock.getSingleChannelBlock(0);
    auto rightBlock = myBlock.getSingleChannelBlock(1);

    auto leftContext = juce::dsp::ProcessContextReplacing<float>(leftBlock);
    auto rightContext = juce::dsp::ProcessContextReplacing<float>(rightBlock);

    // Anti-aliasing filter
    antiAliasingFilter.process(context);
    preGainHighPassFilter.process(context);

    // Pre distortion filters
    preStageLeftFilter.process(leftContext);
    preStageRightFilter.process(rightContext);

    // Stage 1 distortion
    if (!parameters.stage1Bypass)
    {
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            auto* channelData = buffer.getWritePointer(channel);
            for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
            {
                channelData[sample] = stage1Distortion.process(channelData[sample]);
            }
        }

        // Post stage 1 filters
        postStage1LeftFilter.process(leftContext);
        postStage1RightFilter.process(rightContext);

    }
    
    // Stage 2 distortion
    if (!parameters.stage2Bypass)
    {
        if (!parameters.stage1Bypass)
        {
            buffer.applyGain(0.5f);
        }
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            auto* channelData = buffer.getWritePointer(channel);
            for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
            {
                channelData[sample] = stage2Distortion.process(channelData[sample]);
            }
        }

        // Post stage 2 filters
        postStage2LeftFilter.process(leftContext);
        postStage2RightFilter.process(rightContext);
    }

    // Post gain low pass filter to avoid aliasing
    postGainLowPassFilter.process(context);

    // Mid boost filter
    midBoostLeftFilter.process(leftContext);
    midBoostRightFilter.process(rightContext);
}

//==============================================================================
bool GiONAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* GiONAudioProcessor::createEditor()
{
    return new GiONAudioProcessorEditor (*this);
}

//==============================================================================
void GiONAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void GiONAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

juce::AudioProcessorValueTreeState::ParameterLayout GiONAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>("stage1Gain",
                                                           "Stage 1 Gain",
                                                           juce::NormalisableRange<float>(-3.0f, 24.0f, 0.1f),
                                                           0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("stage2Gain",
                                                           "Stage 2 Gain",
                                                           juce::NormalisableRange<float>(-3.0f, 24.0f, 0.1f),
                                                           0.0f));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>("stage1Crunch", 
                                                           "Stage 1 Crunch", 
                                                           juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 
                                                           0.5f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("stage2Crunch",
														   "Stage 2 Crunch",
														   juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
														   0.5f));

    layout.add(std::make_unique<juce::AudioParameterFloat>("stage1Volume",
                                                           "Stage 1 Volume",
                                                           juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f),
                                                           0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("stage2Volume",
                                                           "Stage 2 Volume",
                                                           juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f),
                                                           0.0f));

    // Filters
    layout.add(std::make_unique<juce::AudioParameterFloat>("preStageBassGain", 
                                                           "Pre Stage Bass Gain",
                                                           juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f),
                                                           0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("preStageTrebleGain",
                                                           "Pre Stage Treble Gain",
														   juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f), 
														   0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("postStage1BassGain",
                                                           "Post Stage 1 Bass Gain", 
														   juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f), 
														   0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("postStage1TrebleGain",
                                                           "Post Stage 1 Treble Gain",
														   juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f), 
														   0.0f)); 
    layout.add(std::make_unique<juce::AudioParameterFloat>("postStage2BassGain",
                                                           "Post Stage 2 Bass Gain",
														   juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f), 
														   0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("postStage2TrebleGain",
                                                           "Post Stage 2 Treble Gain",
														   juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f), 
														   0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("midBoostGain",
                                                           "Mid Boost Gain",
														   juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f), 
														   0.0f));

    layout.add(std::make_unique<juce::AudioParameterBool>("stage1Bypass",
                                                          "Stage 1 Bypass",
														   false));

    layout.add(std::make_unique<juce::AudioParameterBool>("stage2Bypass",
                                                          "Stage 2 Bypass",
														   false));
    return layout;
}

parametersStruct getParametersFromTree(juce::AudioProcessorValueTreeState& apvts)
{
    parametersStruct parameters;

    parameters.stage1Crunch = apvts.getRawParameterValue("stage1Crunch")->load();
    parameters.stage2Crunch = apvts.getRawParameterValue("stage2Crunch")->load();
    parameters.stage1Gain = apvts.getRawParameterValue("stage1Gain")->load();
    parameters.stage2Gain = apvts.getRawParameterValue("stage2Gain")->load();
    parameters.stage1Volume = apvts.getRawParameterValue("stage1Volume")->load();
    parameters.stage2Volume = apvts.getRawParameterValue("stage2Volume")->load();

    parameters.preStageBassGain = juce::Decibels::decibelsToGain(apvts.getRawParameterValue("preStageBassGain")->load());
    parameters.preStageTrebleGain = juce::Decibels::decibelsToGain(apvts.getRawParameterValue("preStageTrebleGain")->load());
    parameters.postStage1BassGain = juce::Decibels::decibelsToGain(apvts.getRawParameterValue("postStage1BassGain")->load());
    parameters.postStage1TrebleGain = juce::Decibels::decibelsToGain(apvts.getRawParameterValue("postStage1TrebleGain")->load());
    parameters.postStage2BassGain = juce::Decibels::decibelsToGain(apvts.getRawParameterValue("postStage2BassGain")->load());
    parameters.postStage2TrebleGain = juce::Decibels::decibelsToGain(apvts.getRawParameterValue("postStage2TrebleGain")->load());
    parameters.midBoostGain = juce::Decibels::decibelsToGain(apvts.getRawParameterValue("midBoostGain")->load());

    parameters.stage1Bypass = apvts.getRawParameterValue("stage1Bypass")->load();
    parameters.stage2Bypass = apvts.getRawParameterValue("stage2Bypass")->load();

    return parameters;
}

void GiONAudioProcessor::updateParameters()
{
    auto parameters = getParametersFromTree(apvts);

    stage1Distortion.setCrunch(parameters.stage1Crunch);
    stage2Distortion.setCrunch(parameters.stage2Crunch);
    stage1Distortion.setGain(juce::Decibels::decibelsToGain(parameters.stage1Gain));
    stage2Distortion.setGain(juce::Decibels::decibelsToGain(parameters.stage2Gain));
    stage1Distortion.setVolume(juce::Decibels::decibelsToGain(parameters.stage1Volume));
    stage2Distortion.setVolume(juce::Decibels::decibelsToGain(parameters.stage2Volume));

    
}

void GiONAudioProcessor::updateFilters()
{
    auto parameters = getParametersFromTree(apvts);

	preStageBassCoefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(processSpec.sampleRate,
                                                                                   preStageBassFrequency, 
                                                                                   preStageBassQ, 
                                                                                   parameters.preStageBassGain);
    updateCoefficients(preStageLeftFilter.get<FilterType::bassFilter>().coefficients, preStageBassCoefficients);
    updateCoefficients(preStageRightFilter.get<FilterType::bassFilter>().coefficients, preStageBassCoefficients); 
    
    preStageTrebleCoefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(processSpec.sampleRate, 
                                                                                     preStageTrebleFrequency, 
                                                                                     preStageTrebleQ, 
                                                                                     parameters.preStageTrebleGain);
    updateCoefficients(preStageLeftFilter.get<FilterType::trebleFilter>().coefficients, preStageTrebleCoefficients); 
    updateCoefficients(preStageRightFilter.get<FilterType::trebleFilter>().coefficients, preStageTrebleCoefficients); 

    postStage1BassCoefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(processSpec.sampleRate, 
                                                                                     postStage1BassFrequency, 
                                                                                     postStage1BassQ, 
                                                                                     parameters.postStage1BassGain);
    updateCoefficients(postStage1LeftFilter.get<FilterType::bassFilter>().coefficients, postStage1BassCoefficients);
    updateCoefficients(postStage1RightFilter.get<FilterType::bassFilter>().coefficients, postStage1BassCoefficients); 

    postStage1TrebleCoefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(processSpec.sampleRate, 
                                                                                       postStage1TrebleFrequency, 
                                                                                       postStage1TrebleQ, 
                                                                                       parameters.postStage1TrebleGain);
    updateCoefficients(postStage1LeftFilter.get<FilterType::trebleFilter>().coefficients, postStage1TrebleCoefficients);
    updateCoefficients(postStage1RightFilter.get<FilterType::trebleFilter>().coefficients, postStage1TrebleCoefficients);

    postStage2BassCoefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(processSpec.sampleRate, 
                                                                                     postStage2BassFrequency, 
                                                                                     postStage2BassQ,
                                                                                     parameters.postStage2BassGain);
    updateCoefficients(postStage2LeftFilter.get<FilterType::bassFilter>().coefficients, postStage2BassCoefficients);
    updateCoefficients(postStage2RightFilter.get<FilterType::bassFilter>().coefficients, postStage2BassCoefficients);

    postStage2TrebleCoefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(processSpec.sampleRate,
                                                                                       postStage2TrebleFrequency,
                                                                                       postStage2TrebleQ, 
                                                                                       parameters.postStage2TrebleGain);
    updateCoefficients(postStage2LeftFilter.get<FilterType::trebleFilter>().coefficients, postStage2TrebleCoefficients);
    updateCoefficients(postStage2RightFilter.get<FilterType::trebleFilter>().coefficients, postStage2TrebleCoefficients);

    midBoostCoefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(processSpec.sampleRate,
                                                                               midBoostFrequency,
                                                                               midBoostQ,
                                                                               parameters.midBoostGain);
    updateCoefficients(midBoostLeftFilter.coefficients, midBoostCoefficients);
    updateCoefficients(midBoostRightFilter.coefficients, midBoostCoefficients);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GiONAudioProcessor();
}

