#include "DistortionDSP.h"

DSP::DistortionDSP::DistortionDSP()
{
	this->crunch = 0.0f;
	this->gain = 0.0f;
	this->volume = 0.0f;
	this->distortionStage = 1;
}

DSP::DistortionDSP::~DistortionDSP()
{
}

void DSP::DistortionDSP::setCrunch(float crunch)
{
	this->crunch = crunch;
}

void DSP::DistortionDSP::setGain(float gain)
{
	this->gain = gain;
}

void DSP::DistortionDSP::setVolume(float volume)
{
	this->volume = volume;
}

void DSP::DistortionDSP::setDistortionStage(int stage)
{
	this->distortionStage = stage;
}

float DSP::DistortionDSP::getCrunch()
{
	return this->crunch;
}

float DSP::DistortionDSP::getGain()
{
	return this->gain;
}

float DSP::DistortionDSP::getVolume()
{
	return this->volume;
}

float DSP::DistortionDSP::process(float sample)
{
	// Apply pre distortion gain
	sample = preGainProcessing(sample);

	if (distortionStage == 1)
	{
		if (crunch <= 0.5f)
		{
			sample = interpolateValue(softestDistortion(sample), softDistortion(sample), crunch / 0.5f);
		}
		else
		{
			sample = interpolateValue(softDistortion(sample), mediumDistortion(sample), (crunch - 0.5f) / 0.5f);
		}
	}
	else if (distortionStage == 2)
	{
		if (crunch <= 0.5f)
		{
			sample = interpolateValue(mediumDistortion(sample), hardDistortion(sample), crunch / 0.5f);
		}
		else
		{
			sample = interpolateValue(hardDistortion(sample), hardestDistortion(sample), (crunch - 0.5f) / 0.5f);
		}
	}

	// Apply volume
	sample *= volume;

	return sample;
}

float DSP::DistortionDSP::preGainProcessing(float sample)
{

	return 5.0f * gain * sample;
}

float DSP::DistortionDSP::softestDistortion(float sample)
{
	return (2.0f / pi) * std::atan(pi / 2 * sample);
}

float DSP::DistortionDSP::softDistortion(float sample)
{
	return std::tanh(sample);
}

float DSP::DistortionDSP::mediumDistortion(float sample)
{
	return std::erf(sample * 1.414f);
}

float DSP::DistortionDSP::hardDistortion(float sample)
{
	if (sample >= 0.66f)
	{
		return 1.0f;
	}
	else if (sample <= 0.66f && sample >= 0.33f)
	{
		return (3.0f - std::pow(2.0f - 3.0f * sample, 2)) / 3.0f;
	}
	else if (sample <= 0.33f && sample >= -0.33f)
	{
		return 2.0f * sample;
	}
	else if (sample <= -0.33f && sample >= -0.66f)
	{
		return (-3.0f + std::pow(2.0f + 3.0f * sample, 2)) / 3.0f;
	}
	else
	{
		return -1.0f;
	}
}

float DSP::DistortionDSP::hardestDistortion(float sample)
{
	if (sample >= 0.2f)
	{
		return 1.0f;
	}
	else if (sample <= 0.2f && sample >= -0.2f)
	{
		return 5.0f * sample;
	}
	else
	{
		return -1.0f;
	}
}

float DSP::DistortionDSP::interpolateValue(float valueA, float valueB, float ratio)
{
	return valueA + (valueB - valueA) * ratio;
}


