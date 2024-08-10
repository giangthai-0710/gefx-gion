#include "DistortionDSP.h"

DistortionDSP::DistortionDSP()
{
	this->crunch = 0.0f;
	this->gain = 0.0f;
	this->volume = 0.0f;
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

	// Apply distortion effect
	if (crunch <= 0.2f)
	{
		sample = interpolateValue(sample, softestDistortion(sample), crunch / 0.2f);
	}
	else if (crunch <= 0.4f)
	{
		sample = interpolateValue(softestDistortion(sample), softDistortion(sample), (crunch - 0.2f) / 0.2f);
	}
	else if (crunch <= 0.6f)
	{
		sample = interpolateValue(softDistortion(sample), mediumDistortion(sample), (crunch - 0.4f) / 0.2f);
	}
	else if (crunch <= 0.8f)
	{
		sample = interpolateValue(mediumDistortion(sample), hardDistortion(sample), (crunch - 0.6f) / 0.2f);
	}
	else
	{
		sample = interpolateValue(hardDistortion(sample), hardestDistortion(sample), (crunch - 0.8f) / 0.2f);
	}

	// Apply volume
	sample *= volume;

	return sample;
}
