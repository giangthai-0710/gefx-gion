#pragma once

#include <JuceHeader.h>
#include "LabelLookAndFeel.h"

namespace GUI
{
    class Label : public juce::Label
    {
    public:
        Label();
        ~Label() override;

        void setLabelSize(juce::String size);

    private:
        LabelLookAndFeel labelLookAndFeel;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Label)
    };
}

