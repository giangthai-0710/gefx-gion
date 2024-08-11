#pragma once

#include <JuceHeader.h>

// This class handles the distortion effect
class DistortionDSP
{
public:
    //==============================================================================
    DistortionDSP();
    ~DistortionDSP();

    //==============================================================================
    // Setters and getters
    void setCrunch(float crunch);
    void setGain(float gain);
    void setVolume(float volume);
    void setDistortionStage(int stage);

    float getCrunch();
    float getGain();
    float getVolume();

    //==============================================================================
    // Processes the sample with the distortion effect
    float process(float sample);

private:
    //==============================================================================
    // Distortion parameters
    float crunch;
    float gain;
    float volume;
    
    // Helper variables
    float pi = juce::MathConstants<float>::pi;

    //==============================================================================
    // Processes the sample before applying the distortion effect
    float preGainProcessing(float sample)
    {

        return 5.0f * gain * sample;
    }

    //==============================================================================
    // Clean, soft boost 
    float softestDistortion(float sample)
	{
        return (2.0f / pi) * std::atan(pi / 2 * sample);
	}

    // Light overdrive 
    float softDistortion(float sample)
    {
        
        return std::tanh(sample);
    }

    // Medium soft clipping overdrive
    float mediumDistortion(float sample)
	{      
        return std::erf(sample * 1.414f);
	}

    //==============================================================================
    // Interpolates between two distortion values
    float interpolateValue(float valueA, float valueB, float ratio)
	{
		return valueA + (valueB - valueA) * ratio;
	}
};