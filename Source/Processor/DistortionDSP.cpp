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
