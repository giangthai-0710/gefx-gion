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
    void setCrunch(float crunch);
    void setGain(float gain);
    void setVolume(float volume);

    float getCrunch();
    float getGain();
    float getVolume();

    //==============================================================================
    float process(float sample);

private:
    //==============================================================================
    // Distortion parameters
    float crunch;
    float gain;
    float volume;
    
    const float dcOffset = 0.17f;

    //==============================================================================
    float preGainProcessing(float sample)
    {
        return 5.0f * gain * (sample + dcOffset);
    }

    float softestDistortion(float sample)
	{
		return (2.0f  / juce::MathConstants<float>::pi) * std::atan(juce::MathConstants<float>::pi / 2 * sample);
	}

    float softDistortion(float sample)
    {
        return std::tanh(sample);
    }

    float mediumDistortion(float sample)
	{
        return std::erf(sample * 1.414f);
	}

    float hardDistortion(float sample)
	{
		if(sample > 0.66f)
		{
			return 1.0f;
		}
		else if(sample <= 0.66f && sample >= 0.33f)
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
		else
		{
			return -1.0f;
		}
	}

    float interpolateValue(float valueA, float valueB, float ratio)
	{
		return valueA + (valueB - valueA) * ratio;
	}

};