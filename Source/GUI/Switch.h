#pragma once

#include <JuceHeader.h>

#include "../GUI/SwitchLookAndFeel.h"

namespace GUI
{
    class Switch : public juce::ToggleButton, private juce::Timer
    {
    public:
        Switch();
        ~Switch() override;

        void paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

        void isJustClicked()
        {
            startTimer(100); // Start a timer to reset the button to be unpressed after 100ms
            isPressed = true; // Set the button to be pressed
            repaint(); // Repaint the button
        }

        void mouseEnter(const juce::MouseEvent& event) override;
        
        void mouseExit(const juce::MouseEvent& event) override;

        bool hasMouseOver();

    private:
        bool isMouseOver = false;

        juce::Image switchImage, switchPressedImage;

        bool isPressed = false;

        void timerCallback() override
        {
            stopTimer();
            isPressed = false;
            repaint();
        }

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Switch)
    };
}

