#include <JuceHeader.h>
#include "Label.h"

//==============================================================================
Label::Label()
{
    setLookAndFeel(&labelLookAndFeel);
    setJustificationType(juce::Justification::centred);
    this->setLabelSize("medium");
}

Label::~Label()
{
    setLookAndFeel(nullptr);
}

void Label::setLabelSize(juce::String size)
{
    labelLookAndFeel.setLabelSize(size);
    repaint();
}


