#include "DistortionDSP.h"

DistortionDSP::DistortionDSP()
{
	this->crunch = 0.0f;
	this->gain = 0.0f;
	this->volume = 0.0f;
	this->dcOffset = 0.0f;
	this->tilt = 0.0f;
}

DistortionDSP::~DistortionDSP()
{
}

void DistortionDSP::setCrunch(float crunch)
{
	this->crunch = crunch;
}

void DistortionDSP::setTilt(float tilt)
{
	this->tilt = tilt;
	this->tiltAbsolute = fabs(tilt);
}

void DistortionDSP::setDCOffset(float offset)
{
	this->dcOffset = offset;
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
	if (crunch <= 0.25f)
	{
		sample = interpolateValue(sample, softestDistortion(sample), crunch / 0.25f);
	}
	else if (crunch <= 0.5f)
	{
		sample = interpolateValue(softestDistortion(sample), softDistortion(sample), (crunch - 0.25f) / 0.25f);
	}
	else if (crunch <= 0.75f)
	{
		sample = interpolateValue(softDistortion(sample), hardDistortion(sample), (crunch - 0.5f) / 0.25f);
	}
	else
	{
		sample = hardDistortion(sample);
	}

	// Apply volume
	sample *= volume;

	return sample;
}
