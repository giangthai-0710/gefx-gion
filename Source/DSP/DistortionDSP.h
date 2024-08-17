#pragma once

#include <JuceHeader.h>

// This class handles the distortion effect
namespace DSP
{
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

        int distortionStage;

        // Helper variables
        float pi = juce::MathConstants<float>::pi;

        //==============================================================================
        // Processes the sample before applying the distortion effect
        float preGainProcessing(float sample);

        // Clean, soft boost 
        float softestDistortion(float sample);

        // Light overdrive 
        float softDistortion(float sample);

        // Medium soft clipping overdrive
        float mediumDistortion(float sample);

        // Hard clipping overdrive
        float hardDistortion(float sample);

        // Hardest clipping overdrive
        float hardestDistortion(float sample);

        //==============================================================================
        // Interpolates between two distortion values
        float interpolateValue(float valueA, float valueB, float ratio);
    };
}