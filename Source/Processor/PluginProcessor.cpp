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

    distortionStage.setDistortionStage(1);
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

    preLeftFilter.prepare(processSpec);
    preRightFilter.prepare(processSpec);
    preLeftFilter.reset();
    preRightFilter.reset();

    postLeftFilter.prepare(processSpec);
    postRightFilter.prepare(processSpec);
    postLeftFilter.reset();
    postRightFilter.reset();

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
        buffer.clear(i, 0, buffer.getNumSamples());

    auto parameters = getParametersFromTree(apvts);
    updateParameters();
    updateFilters();

    auto myBlock = juce::dsp::AudioBlock<float>(buffer);
    auto leftBlock = myBlock.getSingleChannelBlock(0);
    auto rightBlock = myBlock.getSingleChannelBlock(1);

    auto myContext = juce::dsp::ProcessContextReplacing<float>(myBlock);
    auto leftContext = juce::dsp::ProcessContextReplacing<float>(leftBlock);
    auto rightContext = juce::dsp::ProcessContextReplacing<float>(rightBlock);

    if (!parameters.distortionBypass)
    {
        // Anti-aliasing filter
		antiAliasingFilter.process(myContext);

        // High-pass filter to remove low frequencies
        preGainHighPassFilter.process(myContext);

        // Pre-distortion tone shaping
        preLeftFilter.process(leftContext);
        preRightFilter.process(rightContext);

        // Distortion stage
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
			auto* channelData = buffer.getWritePointer(channel);
            for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
            {
				channelData[sample] = distortionStage.process(channelData[sample]);
			}
		}

        // Low-pass filter to remove noise
        postGainLowPassFilter.process(myContext);

        // Post-distortion tone shaping
        postLeftFilter.process(leftContext);
        postRightFilter.process(rightContext);

        // Mid boost
        midBoostLeftFilter.process(leftContext);
        midBoostRightFilter.process(rightContext);
	}
}

//==============================================================================
bool GiONAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* GiONAudioProcessor::createEditor()
{
    return new juce::GenericAudioProcessorEditor (*this);
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

    layout.add(std::make_unique<juce::AudioParameterFloat>("gain",
                                                           "Gain",
                                                           juce::NormalisableRange<float>(-3.0f, 15.0f, 0.1f),
                                                           0.0f));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>("crunch", 
                                                           "Crunch", 
                                                           juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 
                                                           0.5f));

    layout.add(std::make_unique<juce::AudioParameterFloat>("volume",
                                                           "Volume",
                                                           juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f),
                                                           0.0f));

    // Filters
    layout.add(std::make_unique<juce::AudioParameterFloat>("preBassGain", 
                                                           "Pre Bass",
                                                           juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f),
                                                           0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("preTrebleGain",
                                                           "Pre Treble",
														   juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f), 
														   0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("postBassGain",
                                                           "Post Bass", 
														   juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f), 
														   0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("postTrebleGain",
                                                           "Post Treble",
														   juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f), 
														   0.0f)); 
    layout.add(std::make_unique<juce::AudioParameterFloat>("midBoostGain",
                                                           "Mid",
														   juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f), 
														   0.0f));

    layout.add(std::make_unique<juce::AudioParameterBool>("distortionBypass",
                                                          "Distortion Bypass",
														   false));

    layout.add(std::make_unique<juce::AudioParameterBool>("distortionStage",
                                                          "Distortion Stage",
														   false));
    return layout;
}

parametersStruct getParametersFromTree(juce::AudioProcessorValueTreeState& apvts)
{
    parametersStruct parameters;

    parameters.distortionGain = juce::Decibels::decibelsToGain(apvts.getRawParameterValue("gain")->load());
    parameters.distortionCrunch = apvts.getRawParameterValue("crunch")->load();
    parameters.distortionVolume = juce::Decibels::decibelsToGain(apvts.getRawParameterValue("volume")->load());

    parameters.preBassGain = juce::Decibels::decibelsToGain(apvts.getRawParameterValue("preBassGain")->load());
    parameters.preTrebleGain = juce::Decibels::decibelsToGain(apvts.getRawParameterValue("preTrebleGain")->load());
    parameters.postBassGain = juce::Decibels::decibelsToGain(apvts.getRawParameterValue("postBassGain")->load());
    parameters.postTrebleGain = juce::Decibels::decibelsToGain(apvts.getRawParameterValue("postTrebleGain")->load());
    parameters.midBoostGain = juce::Decibels::decibelsToGain(apvts.getRawParameterValue("midBoostGain")->load());

    parameters.distortionBypass = apvts.getRawParameterValue("distortionBypass")->load();
    parameters.distortionStage = apvts.getRawParameterValue("distortionStage")->load();

    return parameters;
}

void GiONAudioProcessor::updateParameters()
{
    auto parameters = getParametersFromTree(apvts);

    distortionStage.setGain(parameters.distortionGain);
    distortionStage.setCrunch(parameters.distortionCrunch);
    distortionStage.setVolume(parameters.distortionVolume);

    if (parameters.distortionStage == false)
    {
        distortionStage.setDistortionStage(1);
	}
	else
	{
		distortionStage.setDistortionStage(2);
    }
}

void GiONAudioProcessor::updateFilters()
{
    auto parameters = getParametersFromTree(apvts);

	preBassCoefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(processSpec.sampleRate,
                                                                                   preBassFrequency, 
                                                                                   preBassQ, 
                                                                                   parameters.preBassGain);
    updateCoefficients(preLeftFilter.get<FilterType::bassFilter>().coefficients, preBassCoefficients);
    updateCoefficients(preRightFilter.get<FilterType::bassFilter>().coefficients, preBassCoefficients); 
    
    preTrebleCoefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(processSpec.sampleRate, 
                                                                                     preTrebleFrequency, 
                                                                                     preTrebleQ, 
                                                                                     parameters.preTrebleGain);
    updateCoefficients(preLeftFilter.get<FilterType::trebleFilter>().coefficients, preTrebleCoefficients); 
    updateCoefficients(preRightFilter.get<FilterType::trebleFilter>().coefficients, preTrebleCoefficients); 

    postBassCoefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(processSpec.sampleRate, 
                                                                                     postBassFrequency, 
                                                                                     postBassQ, 
                                                                                     parameters.postBassGain);
    updateCoefficients(postLeftFilter.get<FilterType::bassFilter>().coefficients, postBassCoefficients);
    updateCoefficients(postRightFilter.get<FilterType::bassFilter>().coefficients, postBassCoefficients); 

    postTrebleCoefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(processSpec.sampleRate, 
                                                                                       postTrebleFrequency, 
                                                                                       postTrebleQ, 
                                                                                       parameters.postTrebleGain);
    updateCoefficients(postLeftFilter.get<FilterType::trebleFilter>().coefficients, postTrebleCoefficients);
    updateCoefficients(postRightFilter.get<FilterType::trebleFilter>().coefficients, postTrebleCoefficients);


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

