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
    float tiltAbsolute;
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

    // Hard clipping distortion
    float hardDistortion(float sample)
	{
		if(sample > 0.66f)
		{
			return 1.0f;
		}
		else if(sample <= 0.66f && sample > 0.33f)
		{
			return -3.0f * sample * sample + 4.0f * sample - 1.0f / 3.0f;
		}
		else if (sample < 0.33f && sample >= 0.0f)
		{
			return 2.0f * sample;
		}
        else if (sample < 0.0f && sample >= -0.33f)
		{
			return 2.0f * sample;
		}
		else if (sample < -0.33f && sample >= -0.66f)
		{
			return 3.0f * sample * sample + 4.0f * sample + 1.0f / 3.0f;
		}
		else if (sample < -0.66f)
		{
			return -1.0f;
		}
	}

    // Hard clipping fuzz
    float hardestDistortion(float sample)
    {
        if (sample > 0.2f)
        {
            return 1.0f;
        }
        else if (sample <= -0.2f)
        {
			return -1.0f;
		}
        else
        {
			return 5.0f * sample;
		}
    }

    //==============================================================================
    // Interpolates between two distortion values
    float interpolateValue(float valueA, float valueB, float ratio)
	{
		return valueA + (valueB - valueA) * ratio;
	}
};