#pragma once

#include <JuceHeader.h>

class StageLED : public juce::Component
{
public:
	StageLED();
	~StageLED();

	void paint(juce::Graphics& g) override;
	void resized() override;

	void setIsOn(bool isOn);
	void setDistortionStage(int distortionStage);

private:
	juce::Image ledImage;

	bool isOn = false;
	int distortionStage = 1;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StageLED)
};