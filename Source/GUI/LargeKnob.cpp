#include <JuceHeader.h>
#include "LargeKnob.h"

//==============================================================================
LargeKnob::LargeKnob()
{
    setLookAndFeel(&largeKnobLookAndFeel);
}

LargeKnob::~LargeKnob()
{
    setLookAndFeel(nullptr);
}


