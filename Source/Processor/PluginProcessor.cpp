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
    antiAliasingFilter.setCutoffFrequency(5000.0f);
    antiAliasingFilter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
    antiAliasingFilter.setResonance(0.7071f);

    preGainHighPassFilter.setCutoffFrequency(25.0f);
    preGainHighPassFilter.setType(juce::dsp::StateVariableTPTFilterType::highpass);
    preGainHighPassFilter.setResonance(0.7071f);

    firstPostGainLowPassFilter.setCutoffFrequency(5000.0f);
    firstPostGainLowPassFilter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
    firstPostGainLowPassFilter.setResonance(1.f);

    secondPostGainLowPassFilter.setCutoffFrequency(5000.0f);
    secondPostGainLowPassFilter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
    secondPostGainLowPassFilter.setResonance(1.1f);

    distortionStage.setDistortionStage(1);

    apvts.state.setProperty(Service::PresetManager::presetNameProperty, "", nullptr);

    presetManager = std::make_unique<Service::PresetManager>(apvts);
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

    firstPostGainLowPassFilter.prepare(processSpec);
    firstPostGainLowPassFilter.reset();

    secondPostGainLowPassFilter.prepare(processSpec);
    secondPostGainLowPassFilter.reset();

    preLeftFilter.prepare(processSpec);
    preRightFilter.prepare(processSpec);
    preLeftFilter.reset();
    preRightFilter.reset();

    postLeftFilter.prepare(processSpec);
    postRightFilter.prepare(processSpec);
    postLeftFilter.reset();
    postRightFilter.reset();
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

    // Calculate distortion level before processing
    buffer.applyGain(parameters.distortionGain);
    distortionLevelLeft = buffer.getRMSLevel(0, 0, buffer.getNumSamples());
    distortionLevelRight = buffer.getRMSLevel(1, 0, buffer.getNumSamples());
    distortionLevel = (distortionLevelLeft + distortionLevelRight) / 2;
    buffer.applyGain(1.0f / parameters.distortionGain);


    if (!parameters.distortionBypass)
    {
        // Anti-aliasing filter
        antiAliasingFilter.process(myContext);

        // High-pass filter to remove low frequencies
        preGainHighPassFilter.process(myContext);

        // Pre-distortion tone shaping
        preLeftFilter.process(leftContext);
        preRightFilter.process(rightContext);

        if (parameters.distortionStage == true && parameters.distortionGain >= 3.0f)
        {
            buffer.applyGain(2.0f);
        }

        // Distortion stage
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
			auto* channelData = buffer.getWritePointer(channel);
            for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
            {
				channelData[sample] = distortionStage.process(channelData[sample]);
			}
		}
        // Post-distortion tone shaping
        postLeftFilter.process(leftContext);
        postRightFilter.process(rightContext);

        // Low-pass filter to remove noise
        firstPostGainLowPassFilter.process(myContext);
        secondPostGainLowPassFilter.process(myContext);
	}
}

//==============================================================================
bool GiONAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* GiONAudioProcessor::createEditor()
{
    return new ProcessorEditorWrapper(*this);
}

//==============================================================================
void GiONAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xmlState(state.createXml());
    copyXmlToBinary(*xmlState, destData);
}

void GiONAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
	{
		if (xmlState->hasTagName(apvts.state.getType()))
		{
			apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
            updateParameters();
            updateFilters();
		}
	}
}

juce::AudioProcessorValueTreeState::ParameterLayout GiONAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>("gain",
                                                           "Gain",
                                                           juce::NormalisableRange<float>(-6.0f, 15.0f, 0.1f),
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
    layout.add(std::make_unique<juce::AudioParameterFloat>("preMidGain",
                                                           "Pre Mid",
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
    layout.add(std::make_unique<juce::AudioParameterFloat>("postMidGain",
                                                           "Pre Mid",
                                                           juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f),
                                                           0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("postTrebleGain",
                                                           "Post Treble",
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
    parameters.preMidGain = juce::Decibels::decibelsToGain(apvts.getRawParameterValue("preMidGain")->load());
    parameters.preTrebleGain = juce::Decibels::decibelsToGain(apvts.getRawParameterValue("preTrebleGain")->load());
    parameters.postBassGain = juce::Decibels::decibelsToGain(apvts.getRawParameterValue("postBassGain")->load());
    parameters.postMidGain = juce::Decibels::decibelsToGain(apvts.getRawParameterValue("postMidGain")->load());
    parameters.postTrebleGain = juce::Decibels::decibelsToGain(apvts.getRawParameterValue("postTrebleGain")->load());

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

	preBassCoefficients = juce::dsp::IIR::Coefficients<float>::makeLowShelf(processSpec.sampleRate,
                                                                            bassFrequency, 
                                                                            bassQ, 
                                                                            parameters.preBassGain);
    updateCoefficients(preLeftFilter.get<FilterType::bassFilter>().coefficients, preBassCoefficients);
    updateCoefficients(preRightFilter.get<FilterType::bassFilter>().coefficients, preBassCoefficients);

    preMidCoefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(processSpec.sampleRate, 
                                                                             midFrequency, 
                                                                             midQ, 
                                                                             parameters.preMidGain);
    updateCoefficients(preLeftFilter.get<FilterType::midFilter>().coefficients, preMidCoefficients);
    updateCoefficients(preRightFilter.get<FilterType::midFilter>().coefficients, preMidCoefficients);

    preTrebleCoefficients = juce::dsp::IIR::Coefficients<float>::makeHighShelf(processSpec.sampleRate,
                                                                               trebleFrequency, 
                                                                               trebleQ, 
                                                                               parameters.preTrebleGain);
    updateCoefficients(preLeftFilter.get<FilterType::trebleFilter>().coefficients, preTrebleCoefficients);
    updateCoefficients(preRightFilter.get<FilterType::trebleFilter>().coefficients, preTrebleCoefficients);

    postBassCoefficients = juce::dsp::IIR::Coefficients<float>::makeLowShelf(processSpec.sampleRate,
                                                                             bassFrequency, 
                                                                             bassQ, 
                                                                             parameters.postBassGain);
    updateCoefficients(postLeftFilter.get<FilterType::bassFilter>().coefficients, postBassCoefficients);
    updateCoefficients(postRightFilter.get<FilterType::bassFilter>().coefficients, postBassCoefficients);

    postMidCoefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(processSpec.sampleRate, 
                                                                              midFrequency, 
                                                                              midQ, 
                                                                              parameters.postMidGain);
    updateCoefficients(postLeftFilter.get<FilterType::midFilter>().coefficients, postMidCoefficients);
    updateCoefficients(postRightFilter.get<FilterType::midFilter>().coefficients, postMidCoefficients);

    postTrebleCoefficients = juce::dsp::IIR::Coefficients<float>::makeHighShelf(processSpec.sampleRate,
                                                                                trebleFrequency, 
                                                                                trebleQ, 
                                                                                parameters.postTrebleGain);
    updateCoefficients(postLeftFilter.get<FilterType::trebleFilter>().coefficients, postTrebleCoefficients);
    updateCoefficients(postRightFilter.get<FilterType::trebleFilter>().coefficients, postTrebleCoefficients);


}

float GiONAudioProcessor::getRMSLevel()
{
    parametersStruct parameters = getParametersFromTree(apvts);
    if (parameters.distortionStage == 0)
    {
        return distortionLevel * (1 + parameters.distortionCrunch);
    }
    else
    {
		return distortionLevel * (1 + parameters.distortionCrunch) * 1.5f;
	}
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GiONAudioProcessor();
}

