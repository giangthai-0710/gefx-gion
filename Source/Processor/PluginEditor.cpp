#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GiONAudioProcessorComponent::GiONAudioProcessorComponent (GiONAudioProcessor& p)
    : audioProcessor (p),
      presetPanel(p.getPresetManager())
{
    setLookAndFeel(&generalLNF);

    background = juce::ImageCache::getFromMemory(BinaryData::Background_png, BinaryData::Background_pngSize);

    createKnobs();

    createSwitches();

    addAndMakeVisible(presetPanel);
    addAndMakeVisible(tooltipTextBox);

    startTimerHz(30);
}

GiONAudioProcessorComponent::~GiONAudioProcessorComponent()
{
       
}

void GiONAudioProcessorComponent::timerCallback()
{
    float percentage = (audioProcessor.getRMSLevel() >= 1) ? 1 : audioProcessor.getRMSLevel();
    gainLED.setGainPercentage(percentage);

    setTooltipText();
}


void GiONAudioProcessorComponent::createKnobs()
{
    // Create knobs and labels
    gainKnob.create(-6.0f, 15.0f, 0.1f, 0.0f,
                    "small", "volume",
                    "Adjust the input level of the signal");
    gainLabel.setText("GAIN", juce::dontSendNotification);

    crunchKnob.create(0.0f, 1.0f, 0.01f, 0.5f, 
                      "large", "null", 
                      "Adjust the distortion level applied to the signal");
    crunchLabel.setText("CRUNCH", juce::dontSendNotification);

    volumeKnob.create(-12.0f, 12.0f, 0.1f, 0.0f,
                      "small", "volume",
                      "Adjust the output level of the signal");
    volumeLabel.setText("VOLUME", juce::dontSendNotification);

    preBassKnob.create(-12.0f, 12.0f, 0.1f, 0.0f,
                       "medium", "volume", 
                       "Adjust the bass response of the signal before distortion");
    preBassLabel.setText("PRE BASS", juce::dontSendNotification);

    preMidKnob.create(-12.0f, 12.0f, 0.1f, 0.0f, 
                      "medium", "volume", 
                      "Adjust the mid response of the signal before distortion");
    preMidLabel.setText("PRE MID", juce::dontSendNotification);

    preTrebleKnob.create(-12.0f, 12.0f, 0.1f, 0.0f,
                         "medium", "volume", 
                         "Adjust the treble response of the signal before distortion");
    preTrebleLabel.setText("PRE TREB", juce::dontSendNotification);

    postBassKnob.create(-12.0f, 12.0f, 0.1f, 0.0f,
                        "medium", "volume", 
                        "Adjust the bass response of the signal after distortion");
    postBassLabel.setText("POST BASS", juce::dontSendNotification);

    postMidKnob.create(-12.0f, 12.0f, 0.1f, 0.0f, 
                       "medium", "volume", 
                       "Adjust the mid response of the signal after distortion");
    postMidLabel.setText("POST MID", juce::dontSendNotification);

    postTrebleKnob.create(-12.0f, 12.0f, 0.1f, 0.0f, 
                          "medium", "volume", 
                          "Adjust the treble response of the signal after distortion");
    postTrebleLabel.setText("POST TREB", juce::dontSendNotification);

    // Add attachments
    gainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "gain", gainKnob);
    crunchAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "crunch", crunchKnob);
    volumeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "volume", volumeKnob);
    preBassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "preBassGain", preBassKnob);
    preMidAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "preMidGain", preMidKnob);
    preTrebleAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "preTrebleGain", preTrebleKnob);
    postBassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "postBassGain", postBassKnob);
    postMidAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "postMidGain", postMidKnob);
    postTrebleAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "postTrebleGain", postTrebleKnob);

    // Add components to the main window
    addAndMakeVisible(gainKnob);
    addAndMakeVisible(gainLabel);

    addAndMakeVisible(crunchKnob);
    addAndMakeVisible(crunchLabel);

    addAndMakeVisible(volumeKnob);
    addAndMakeVisible(volumeLabel);

    addAndMakeVisible(preBassKnob);
    addAndMakeVisible(preBassLabel);

    addAndMakeVisible(preMidKnob);
    addAndMakeVisible(preMidLabel);

    addAndMakeVisible(preTrebleKnob);
    addAndMakeVisible(preTrebleLabel);

    addAndMakeVisible(postBassKnob);
    addAndMakeVisible(postBassLabel);

    addAndMakeVisible(postMidKnob);
    addAndMakeVisible(postMidLabel);

    addAndMakeVisible(postTrebleKnob);
    addAndMakeVisible(postTrebleLabel);
}

void GiONAudioProcessorComponent::createSwitches()
{
    // Bypass switch
    bypassSwitch.setDescription("Bypass the distortion effect");

    if (audioProcessor.apvts.getRawParameterValue("distortionBypass")->load() == 1)
    {
        bypassSwitch.setToggleState(false, juce::dontSendNotification);
        gainLED.setIsOn(true);
        stageLED.setIsOn(true);
    }
    else
    {
        bypassSwitch.setToggleState(true, juce::dontSendNotification);
        gainLED.setIsOn(false);
        stageLED.setIsOn(false);
    }

    bypassSwitch.onClick = [this]
        {
            gainLED.setIsOn(!bypassSwitch.getToggleState());
            stageLED.setIsOn(!bypassSwitch.getToggleState());
            bypassSwitch.isJustClicked();
        };

    bypassLabel.setText("BYPASS", juce::dontSendNotification);

    // Change stage switch
    changeStageSwitch.setDescription("Change the distortion stage");

    if (audioProcessor.apvts.getRawParameterValue("distortionStage")->load() == 1)
    {
        changeStageSwitch.setToggleState(false, juce::dontSendNotification);
        stageLED.setDistortionStage(2);
    }
    else
    {
        changeStageSwitch.setToggleState(true, juce::dontSendNotification);
        stageLED.setDistortionStage(1);
    }

    changeStageSwitch.setToggleState(true, juce::dontSendNotification);
    changeStageSwitch.onClick = [this]
        {
            stageLED.setDistortionStage(!changeStageSwitch.getToggleState() ? 1 : 2);
            changeStageSwitch.isJustClicked();
        };

    changeStageLabel.setText("STAGE", juce::dontSendNotification);

    // Add attachments
    bypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "distortionBypass", bypassSwitch);
    changeStageAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "distortionStage", changeStageSwitch);

    // Add components to the main window
    addAndMakeVisible(bypassSwitch);
    addAndMakeVisible(bypassLabel);

    addAndMakeVisible(changeStageSwitch);
    addAndMakeVisible(changeStageLabel);

    addAndMakeVisible(stageLED);
    addAndMakeVisible(gainLED);
}

void GiONAudioProcessorComponent::setTooltipText()
{
    if (gainKnob.hasMouseOver())
    {
        tooltipTextBox.displayKnobDescription(gainKnob);
    }
    else if (crunchKnob.hasMouseOver())
    {
        tooltipTextBox.displayKnobDescription(crunchKnob);
    }
    else if (volumeKnob.hasMouseOver())
    {
		tooltipTextBox.displayKnobDescription(volumeKnob);
	}
    else if (preBassKnob.hasMouseOver())
    {
		tooltipTextBox.displayKnobDescription(preBassKnob);
	}
    else if (preMidKnob.hasMouseOver())
    {
		tooltipTextBox.displayKnobDescription(preMidKnob);
	}
    else if (preTrebleKnob.hasMouseOver())
    {
		tooltipTextBox.displayKnobDescription(preTrebleKnob);
	}
    else if (postBassKnob.hasMouseOver())
    {
		tooltipTextBox.displayKnobDescription(postBassKnob);
	}
    else if (postMidKnob.hasMouseOver())
    {
		tooltipTextBox.displayKnobDescription(postMidKnob);
	}
    else if (postTrebleKnob.hasMouseOver())
    {
		tooltipTextBox.displayKnobDescription(postTrebleKnob);
	}
    else if (bypassSwitch.hasMouseOver())
    {
		tooltipTextBox.displaySwitchDescription(bypassSwitch);
	}
    else if (changeStageSwitch.hasMouseOver())
    {
		tooltipTextBox.displaySwitchDescription(changeStageSwitch);
	}
    else
    {
		tooltipTextBox.clear();
	}
}

//==============================================================================
void GiONAudioProcessorComponent::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff1A1A1A));
    g.drawImage(background, 0, 35, 480, 270, 0, 0, background.getWidth(), background.getHeight());
}

void GiONAudioProcessorComponent::resized()
{
    // Window size and margins
    auto windowWidth = getWidth();
    auto windowHeight = getHeight();
    
    auto verticalMargin = 10;
    auto horizontalMargin = 10;

    // Bounds for all the components including tooltips and save/load buttons
    auto topLine = verticalMargin;
    auto bottomLine = windowHeight - verticalMargin;
    auto leftLine = horizontalMargin;
    auto rightLine = windowWidth - horizontalMargin;

    // Bound for the plugin components
    auto pluginTopLine = topLine + 35;
    auto pluginBottomLine = bottomLine - 55; 
    auto pluginLeftLine = leftLine;
    auto pluginRightLine = rightLine;

    // We divide the window in 8 vertical sections and 6 horizontal sections
    auto firstVerticalEighthLine = leftLine + (rightLine - leftLine) / 8;
    auto secondVerticalEighthLine = leftLine + (rightLine - leftLine) / 8 * 2;
    auto thirdVerticalEighthLine = leftLine + (rightLine - leftLine) / 8 * 3;
    auto fourthVerticalEighthLine = leftLine + (rightLine - leftLine) / 8 * 4;
    auto fifthVerticalEighthLine = leftLine + (rightLine - leftLine) / 8 * 5;
    auto sixthVerticalEighthLine = leftLine + (rightLine - leftLine) / 8 * 6;
    auto seventhVerticalEighthLine = leftLine + (rightLine - leftLine) / 8 * 7;

    auto firstHorizontalSixthLine = pluginTopLine + (pluginBottomLine - pluginTopLine) / 6;
    auto secondHorizontalSixthLine = pluginTopLine + (pluginBottomLine - pluginTopLine) / 6 * 2;
    auto thirdHorizontalSixthLine = pluginTopLine + (pluginBottomLine - pluginTopLine) / 6 * 3;
    auto fourthHorizontalSixthLine = pluginTopLine + (pluginBottomLine - pluginTopLine) / 6 * 4;
    auto fifthHorizontalSixthLine = pluginTopLine + (pluginBottomLine - pluginTopLine) / 6 * 5;


    // Knobs and labels dimensions
    auto largeKnobWidth = 96;
    auto largeKnobHeight = 120;

    auto mediumKnobWidth = 64;
    auto mediumKnobHeight = 80;

    auto smallKnobWidth = 48;
    auto smallKnobHeight = 60;

    auto labelHeight = 15;
    auto labelWidth = 64;

    auto switchWidth = 48;
    auto switchHeight = 48;

    auto ledWidth = 64;
    auto ledHeight = 64;

    // Pre calculated values to make the code more readable
    auto largeKnobDeltaX = largeKnobWidth / 2; // This is the distance from the center of the knob to the left edge, used to center the knob
    auto mediumKnobDeltaX = mediumKnobWidth / 2;
    auto smallKnobDeltaX = smallKnobWidth / 2;
    auto labelDeltaX = labelWidth / 2; // This is the distance from the center of the label to the left edge, used to center the label

    auto largeKnobDeltaY = largeKnobHeight / 2; // This is the distance from the center of the knob to the top edge, used to center the knob
    auto mediumKnobDeltaY = mediumKnobHeight / 2;
    auto smallKnobDeltaY = smallKnobHeight / 2;
    auto labelDeltaY = labelHeight / 2; // This is the distance from the center of the label to the top edge, used to center the label

    auto largeKnobLabelDeltaY = largeKnobHeight / 2 - largeKnobHeight / 8; // This is the distance from the center of the knob to the top edge of the label, used to center the label
    auto mediumKnobLabelDeltaY = mediumKnobHeight / 2 - mediumKnobHeight / 8;
    auto smallKnobLabelDeltaY = smallKnobHeight / 2 - smallKnobHeight / 8;

    auto switchDeltaX = switchWidth / 2;
    auto switchDeltaY = switchHeight / 2;

    auto ledDeltaX = ledWidth / 2;
    auto ledDeltaY = ledHeight / 2 - 10; // We subtract 10 since the LED is just 10 pixels, the LED png is large due to the glow effect

    // Knobs and labels positions
    gainKnob.setBounds(secondVerticalEighthLine, secondHorizontalSixthLine, smallKnobWidth, smallKnobHeight);
    gainLabel.setBounds(secondVerticalEighthLine + smallKnobDeltaX - labelDeltaX, secondHorizontalSixthLine + smallKnobDeltaY + smallKnobLabelDeltaY, labelWidth, labelHeight);

    crunchKnob.setBounds(fourthVerticalEighthLine - largeKnobDeltaX, thirdHorizontalSixthLine - largeKnobDeltaY, largeKnobWidth, largeKnobHeight);
    crunchLabel.setBounds(fourthVerticalEighthLine - labelDeltaX, thirdHorizontalSixthLine + largeKnobLabelDeltaY, labelWidth, labelHeight);

    volumeKnob.setBounds(sixthVerticalEighthLine - smallKnobWidth, secondHorizontalSixthLine, smallKnobWidth, smallKnobHeight);
    volumeLabel.setBounds(sixthVerticalEighthLine - smallKnobDeltaX - labelDeltaX, secondHorizontalSixthLine + smallKnobDeltaY + smallKnobLabelDeltaY, labelWidth, labelHeight);
    
    preBassKnob.setBounds(leftLine, thirdHorizontalSixthLine - mediumKnobDeltaY, mediumKnobWidth, mediumKnobHeight);
    preBassLabel.setBounds(leftLine, thirdHorizontalSixthLine + mediumKnobLabelDeltaY, labelWidth, labelHeight);

    preMidKnob.setBounds(leftLine, pluginTopLine, mediumKnobWidth, mediumKnobHeight);
    preMidLabel.setBounds(leftLine, firstHorizontalSixthLine + mediumKnobLabelDeltaY, labelWidth, labelHeight);

    preTrebleKnob.setBounds(thirdVerticalEighthLine - mediumKnobDeltaX, pluginTopLine, mediumKnobWidth, mediumKnobHeight);
    preTrebleLabel.setBounds(thirdVerticalEighthLine - labelDeltaX, firstHorizontalSixthLine + mediumKnobLabelDeltaY, labelWidth, labelHeight);

    postBassKnob.setBounds(rightLine - mediumKnobWidth, thirdHorizontalSixthLine - mediumKnobDeltaY, mediumKnobWidth, mediumKnobHeight);
    postBassLabel.setBounds(rightLine - mediumKnobWidth, thirdHorizontalSixthLine + mediumKnobLabelDeltaY, labelWidth, labelHeight);

    postMidKnob.setBounds(rightLine - mediumKnobWidth, pluginTopLine, mediumKnobWidth, mediumKnobHeight);
    postMidLabel.setBounds(rightLine - mediumKnobDeltaX - labelDeltaX, firstHorizontalSixthLine + mediumKnobLabelDeltaY, labelWidth, labelHeight);

    postTrebleKnob.setBounds(fifthVerticalEighthLine - mediumKnobDeltaX, pluginTopLine, mediumKnobWidth, mediumKnobHeight);
    postTrebleLabel.setBounds(fifthVerticalEighthLine - labelDeltaX, firstHorizontalSixthLine + mediumKnobLabelDeltaY, labelWidth, labelHeight);

    bypassSwitch.setBounds(seventhVerticalEighthLine - switchDeltaX, fifthHorizontalSixthLine - switchDeltaY, switchWidth, switchHeight);
    bypassLabel.setBounds(seventhVerticalEighthLine - labelDeltaX, fifthHorizontalSixthLine + switchDeltaY, labelWidth, labelHeight);

    changeStageSwitch.setBounds(firstVerticalEighthLine - switchDeltaX, fifthHorizontalSixthLine - switchDeltaY, switchWidth, switchHeight);
    changeStageLabel.setBounds(firstVerticalEighthLine - labelDeltaX, fifthHorizontalSixthLine + switchDeltaY, labelWidth, labelHeight);

    gainLED.setBounds(seventhVerticalEighthLine - ledDeltaX, fourthHorizontalSixthLine - ledDeltaY, ledWidth, ledHeight);
    stageLED.setBounds(firstVerticalEighthLine - ledDeltaX, fourthHorizontalSixthLine - ledDeltaY, ledWidth, ledHeight);

    presetPanel.setBounds(0, 0, rightLine - leftLine, 30); 

    tooltipTextBox.setBounds(leftLine, pluginBottomLine, rightLine - leftLine, 50);
}

ProcessorEditorWrapper::ProcessorEditorWrapper(GiONAudioProcessor& p) :
    AudioProcessorEditor(p), 
    processorComponent(p)
{
    addAndMakeVisible(processorComponent);

    juce::PropertiesFile::Options options;
    options.applicationName = ProjectInfo::projectName;
    options.commonToAllUsers = true;
    options.filenameSuffix = ".settings";
    
    applicationProperties.setStorageParameters(options);
	
    if (auto* constrainer = getConstrainer())
    {
		constrainer->setFixedAspectRatio(static_cast<double> (originalWidth) / static_cast<double> (originalHeight));
        constrainer->setSizeLimits( originalHeight / 2, originalWidth / 2, originalHeight * 2, originalWidth * 2);
	}

    auto scaleFactor = 1.0;

    if (auto* properties = applicationProperties.getCommonSettings(true))
    {
        scaleFactor = properties->getDoubleValue("scaleFactor", 1.0);
    }

    setResizable(true, true);
    setSize(static_cast<int>(originalWidth * scaleFactor), static_cast<int>(originalHeight * scaleFactor));
}

void ProcessorEditorWrapper::resized()
{
    const auto scaleFactor = static_cast<float>(getWidth()) / static_cast<float>(originalWidth);

    if (auto* properties = applicationProperties.getCommonSettings(true))
    {
        properties->setValue("scaleFactor", scaleFactor);
    }

    processorComponent.setTransform(juce::AffineTransform::scale(scaleFactor));
    processorComponent.setBounds(0, 0, originalWidth, originalHeight);

}
