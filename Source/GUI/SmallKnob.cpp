#include <JuceHeader.h>
#include "SmallKnob.h"

//==============================================================================
SmallKnob::SmallKnob()
{
    setLookAndFeel(&smallKnobLookAndFeel);
}

SmallKnob::~SmallKnob()
{
    setLookAndFeel(nullptr);
}
