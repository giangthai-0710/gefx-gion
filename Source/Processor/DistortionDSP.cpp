#include "DistortionDSP.h"

DistortionDSP::DistortionDSP()
{
	this->crunch = 0.0f;
	this->gain = 0.0f;
	this->volume = 0.0f;
	this->distortionStage = 1;
}

DistortionDSP::~DistortionDSP()
{
}

void DistortionDSP::setCrunch(float crunch)
{
	this->crunch = crunch;
}

void DistortionDSP::setGain(float gain)
{
	this->gain = gain;
}

void DistortionDSP::setVolume(float volume)
{
	this->volume = volume;
}

void DistortionDSP::setDistortionStage(int stage)
{
	this->distortionStage = stage;
}

float DistortionDSP::getCrunch()
{
	return this->crunch;
}

float DistortionDSP::getGain()
{
	return this->gain;
}

float DistortionDSP::getVolume()
{
	return this->volume;
}

float DistortionDSP::process(float sample)
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
