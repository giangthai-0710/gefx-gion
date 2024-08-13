/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GiONAudioProcessorEditor::GiONAudioProcessorEditor (GiONAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),
      presetPanel(p.getPresetManager())
{
    // Set the window title
    setTitle("GiON");
    // Set the window size
    setSize (480, 360);

    background = juce::ImageCache::getFromMemory(BinaryData::Background_png, BinaryData::Background_pngSize);

    // Gain knob
    gainKnob.setRange(-6.0f, 15.0f, 0.1f);
    gainKnob.setValue(0.0f);
    gainKnob.setKnobSize("small");
    gainKnob.setKnobType("volume");

    gainLabel.setText("GAIN", juce::dontSendNotification);

    // Crunch knob
    crunchKnob.setRange(0.0f, 1.0f, 0.01f);
    crunchKnob.setValue(0.5f);
    crunchKnob.setKnobSize("large");
    crunchKnob.setKnobType("null");

    crunchLabel.setText("CRUNCH", juce::dontSendNotification);

    // Volume knob
    volumeKnob.setRange(-12.0f, 12.0f, 0.1f);
    volumeKnob.setValue(0.0f);
    volumeKnob.setKnobSize("small");
    volumeKnob.setKnobType("volume");

    volumeLabel.setText("VOLUME", juce::dontSendNotification);

    // Pre bass knob
    preBassKnob.setRange(-12.0f, 12.0f, 0.1f);
    preBassKnob.setValue(0.0f);
    preBassKnob.setKnobSize("medium");
    preBassKnob.setKnobType("volume");

    preBassLabel.setText("PRE BASS", juce::dontSendNotification);

    // Pre mid knob
    preMidKnob.setRange(-12.0f, 12.0f, 0.1f);
    preMidKnob.setValue(0.0f);
    preMidKnob.setKnobSize("medium");
    preMidKnob.setKnobType("volume");

    preMidLabel.setText("PRE MID", juce::dontSendNotification);

    // Pre treble knob
    preTrebleKnob.setRange(-12.0f, 12.0f, 0.1f);
    preTrebleKnob.setValue(0.0f);
    preTrebleKnob.setKnobSize("medium");
    preTrebleKnob.setKnobType("volume");

    preTrebleLabel.setText("PRE TREB", juce::dontSendNotification);

    // Post bass knob
    postBassKnob.setRange(-12.0f, 12.0f, 0.1f);
    postBassKnob.setValue(0.0f);
    postBassKnob.setKnobSize("medium");
    postBassKnob.setKnobType("volume");

    postBassLabel.setText("POST BASS", juce::dontSendNotification);

    // Post mid knob
    postMidKnob.setRange(-12.0f, 12.0f, 0.1f);
    postMidKnob.setValue(0.0f);
    postMidKnob.setKnobSize("medium");
    postMidKnob.setKnobType("volume");

    postMidLabel.setText("POST MID", juce::dontSendNotification);

    // Post treble knob
    postTrebleKnob.setRange(-12.0f, 12.0f, 0.1f);
    postTrebleKnob.setValue(0.0f);
    postTrebleKnob.setKnobSize("medium");
    postTrebleKnob.setKnobType("volume");

    postTrebleLabel.setText("POST TREB", juce::dontSendNotification);

    // Bypass switch
    if (p.apvts.getRawParameterValue("distortionBypass")->load() == 1)
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

    // Change stage switch
    if (p.apvts.getRawParameterValue("distortionStage")->load() == 1)
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
    gainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "gain", gainKnob);
    crunchAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "crunch", crunchKnob);
    volumeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "volume", volumeKnob);
    preBassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "preBassGain", preBassKnob);
    preMidAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "preMidGain", preMidKnob);
    preTrebleAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "preTrebleGain", preTrebleKnob);
    postBassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "postBassGain", postBassKnob);
    postMidAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "postMidGain", postMidKnob);
    postTrebleAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "postTrebleGain", postTrebleKnob);

    bypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "distortionBypass", bypassSwitch);
    changeStageAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "distortionStage", changeStageSwitch);

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

    addAndMakeVisible(bypassSwitch);
    addAndMakeVisible(bypassLabel);

    addAndMakeVisible(changeStageSwitch);
    addAndMakeVisible(changeStageLabel);

    addAndMakeVisible(stageLED);
    addAndMakeVisible(gainLED);

    addAndMakeVisible(presetPanel);

    startTimerHz(30);
}

GiONAudioProcessorEditor::~GiONAudioProcessorEditor()
{
       
}

void GiONAudioProcessorEditor::timerCallback()
{
    float percentage = (audioProcessor.getRMSLevel() >= 1) ? 1 : audioProcessor.getRMSLevel();
    gainLED.setGainPercentage(percentage);
}

//==============================================================================
void GiONAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff1A1A1A));
    g.drawImage(background, 0, 35, 480, 270, 0, 0, background.getWidth(), background.getHeight());
}

void GiONAudioProcessorEditor::resized()
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
}

