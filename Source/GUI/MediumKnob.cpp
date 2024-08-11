#include <JuceHeader.h>
#include "MediumKnob.h"

//==============================================================================
MediumKnob::MediumKnob()
{
    setLookAndFeel(&mediumKnobLookAndFeel);
}

MediumKnob::~MediumKnob()
{
	setLookAndFeel(nullptr);
}



