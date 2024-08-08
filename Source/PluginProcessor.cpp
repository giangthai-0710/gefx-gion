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
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
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

    updateParameters();

    // This is where the audio processing happens
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
		{
			channelData[sample] = stage1Distortion.process(channelData[sample]);
            channelData[sample] = stage2Distortion.process(channelData[sample]);
		}
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

    layout.add(std::make_unique<juce::AudioParameterFloat>("stage1Gain",
                                                           "Stage 1 Gain",
                                                           juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
                                                           0.5f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("stage2Gain",
                                                           "Stage 2 Gain",
                                                           juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
                                                           0.5f));
    
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
                                                           juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
                                                           0.5f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("stage2Volume",
                                                           "Stage 2 Volume",
                                                           juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
                                                           0.5f));

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

    return parameters;
}

void GiONAudioProcessor::updateParameters()
{
    auto parameters = getParametersFromTree(apvts);

    stage1Distortion.setCrunch(parameters.stage1Crunch);
    stage2Distortion.setCrunch(parameters.stage2Crunch);
    stage1Distortion.setGain(parameters.stage1Gain);
    stage2Distortion.setGain(parameters.stage2Gain);
    stage1Distortion.setVolume(parameters.stage1Volume);
    stage2Distortion.setVolume(parameters.stage2Volume);

}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GiONAudioProcessor();
}

