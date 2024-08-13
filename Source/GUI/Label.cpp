#include <JuceHeader.h>
#include "Label.h"

//==============================================================================
GUI::Label::Label()
{
    setLookAndFeel(&labelLookAndFeel);
    setJustificationType(juce::Justification::centred);
    this->setLabelSize("medium");
}

GUI::Label::~Label()
{
    setLookAndFeel(nullptr);
}

void GUI::Label::setLabelSize(juce::String size)
{
    labelLookAndFeel.setLabelSize(size);
    repaint();
}


