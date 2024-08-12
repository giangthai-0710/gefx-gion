/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GiONAudioProcessorEditor::GiONAudioProcessorEditor (GiONAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Set the window title
    setTitle("GiON");
    // Set the window size
    setSize (480, 270);

    getLookAndFeel().getDefaultLookAndFeel().setDefaultSansSerifTypefaceName("Arial");

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

    preBassLabel.setText("BASS", juce::dontSendNotification);

    // Pre mid knob
    preMidKnob.setRange(-12.0f, 12.0f, 0.1f);
    preMidKnob.setValue(0.0f);
    preMidKnob.setKnobSize("medium");
    preMidKnob.setKnobType("volume");

    preMidLabel.setText("MID", juce::dontSendNotification);

    // Pre treble knob
    preTrebleKnob.setRange(-12.0f, 12.0f, 0.1f);
    preTrebleKnob.setValue(0.0f);
    preTrebleKnob.setKnobSize("medium");
    preTrebleKnob.setKnobType("volume");

    preTrebleLabel.setText("TREBLE", juce::dontSendNotification);

    // Post bass knob
    postBassKnob.setRange(-12.0f, 12.0f, 0.1f);
    postBassKnob.setValue(0.0f);
    postBassKnob.setKnobSize("medium");
    postBassKnob.setKnobType("volume");

    postBassLabel.setText("BASS", juce::dontSendNotification);

    // Post mid knob
    postMidKnob.setRange(-12.0f, 12.0f, 0.1f);
    postMidKnob.setValue(0.0f);
    postMidKnob.setKnobSize("medium");
    postMidKnob.setKnobType("volume");

    postMidLabel.setText("MID", juce::dontSendNotification);

    // Post treble knob
    postTrebleKnob.setRange(-12.0f, 12.0f, 0.1f);
    postTrebleKnob.setValue(0.0f);
    postTrebleKnob.setKnobSize("medium");
    postTrebleKnob.setKnobType("volume");

    postTrebleLabel.setText("TREBLE", juce::dontSendNotification);

    // Bypass switch
    bypassSwitch.setToggleState(true, juce::dontSendNotification);
    bypassSwitch.onClick = [this]
    {
        gainLED.setIsOn(!bypassSwitch.getToggleState());
		stageLED.setIsOn(!bypassSwitch.getToggleState());
        bypassSwitch.isJustClicked();
	};

	// Change stage switch
    changeStageSwitch.setToggleState(true, juce::dontSendNotification);
	changeStageSwitch.onClick = [this]
	{
		stageLED.setDistortionStage(!changeStageSwitch.getToggleState() ? 1 : 2);
        changeStageSwitch.isJustClicked();
	};

    // LED
    stageLED.setIsOn(bypassSwitch.getToggleState());
    stageLED.setDistortionStage(1);

    gainLED.setIsOn(bypassSwitch.getToggleState());
    gainLED.setGainPercentage(0.5f);
    

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
    addAndMakeVisible(changeStageSwitch);

    addAndMakeVisible(stageLED);
    addAndMakeVisible(gainLED);

    startTimerHz(30);
}

GiONAudioProcessorEditor::~GiONAudioProcessorEditor()
{
    
}

void GiONAudioProcessorEditor::timerCallback()
{
    float percentage = (audioProcessor.getRMSLevel() >= 1) ? 1 : audioProcessor.getRMSLevel();
    DBG(percentage);
    gainLED.setGainPercentage(percentage);
}

//==============================================================================
void GiONAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colour(0xff303030));
}

void GiONAudioProcessorEditor::resized()
{
    // Window size and margins
    auto windowWidth = getWidth();
    auto windowHeight = getHeight();
    
    auto verticalMargin = 10;
    auto horizontalMargin = 10;

    // General grid lines
    auto topLine = verticalMargin;
    auto bottomLine = windowHeight - verticalMargin;
    auto leftLine = horizontalMargin;
    auto rightLine = windowWidth - horizontalMargin;

    auto verticalMiddleLine = windowWidth / 2;
    auto horizontalMiddleLine = windowHeight / 2;

    auto firstVerticalQuarterLine = windowWidth / 4;
    auto thirdVerticalQuarterLine = firstVerticalQuarterLine * 3;

    auto firstVerticalEighthLine = windowWidth / 8;
    auto secondVerticalEighthLine = firstVerticalEighthLine * 2;
    auto thirdVerticalEighthLine = firstVerticalEighthLine * 3;
    auto fifthVerticalEighthLine = firstVerticalEighthLine * 5;
    auto sixthVerticalEighthLine = firstVerticalEighthLine * 6;
    auto seventhVerticalEighthLine = firstVerticalEighthLine * 7;

    auto firstHorizontalSixthLine = windowHeight / 6;
    auto secondHorizontalSixthLine = firstHorizontalSixthLine * 2;
    auto fourthHorizontalSixthLine = firstHorizontalSixthLine * 4;
    auto lastHorizontalSixthLine = firstHorizontalSixthLine * 5;

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

    auto ledWidth = 24;
    auto ledHeight = 24;

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
    auto ledDeltaY = ledHeight / 2;

    // Knobs and labels positions
    gainKnob.setBounds(firstVerticalQuarterLine, secondHorizontalSixthLine, smallKnobWidth, smallKnobHeight);
    gainLabel.setBounds(firstVerticalQuarterLine + smallKnobDeltaX - labelDeltaX, secondHorizontalSixthLine + smallKnobDeltaY + smallKnobLabelDeltaY, labelWidth, labelHeight);

    crunchKnob.setBounds(verticalMiddleLine - largeKnobDeltaX, horizontalMiddleLine - largeKnobDeltaY, largeKnobWidth, largeKnobHeight);
    crunchLabel.setBounds(verticalMiddleLine - labelDeltaX, horizontalMiddleLine + largeKnobLabelDeltaY, labelWidth, labelHeight);

    volumeKnob.setBounds(thirdVerticalQuarterLine - smallKnobWidth, secondHorizontalSixthLine, smallKnobWidth, smallKnobHeight);
    volumeLabel.setBounds(thirdVerticalQuarterLine - smallKnobDeltaX - labelDeltaX, secondHorizontalSixthLine + smallKnobDeltaY + smallKnobLabelDeltaY, labelWidth, labelHeight);
    
    preBassKnob.setBounds(leftLine, horizontalMiddleLine - mediumKnobDeltaY, mediumKnobWidth, mediumKnobHeight);
    preBassLabel.setBounds(leftLine, horizontalMiddleLine + mediumKnobLabelDeltaY, labelWidth, labelHeight);

    preMidKnob.setBounds(leftLine, topLine, mediumKnobWidth, mediumKnobHeight);
    preMidLabel.setBounds(leftLine, firstHorizontalSixthLine + mediumKnobLabelDeltaY, labelWidth, labelHeight);

    preTrebleKnob.setBounds(thirdVerticalEighthLine - mediumKnobDeltaX, topLine, mediumKnobWidth, mediumKnobHeight);
    preTrebleLabel.setBounds(thirdVerticalEighthLine - labelDeltaX, firstHorizontalSixthLine + mediumKnobLabelDeltaY, labelWidth, labelHeight);

    postBassKnob.setBounds(rightLine - mediumKnobWidth, horizontalMiddleLine - mediumKnobDeltaY, mediumKnobWidth, mediumKnobHeight);
    postBassLabel.setBounds(rightLine - mediumKnobWidth, horizontalMiddleLine + mediumKnobLabelDeltaY, labelWidth, labelHeight);

    postMidKnob.setBounds(rightLine - mediumKnobWidth, topLine, mediumKnobWidth, mediumKnobHeight);
    postMidLabel.setBounds(rightLine - mediumKnobDeltaX - labelDeltaX, firstHorizontalSixthLine + mediumKnobLabelDeltaY, labelWidth, labelHeight);

    postTrebleKnob.setBounds(fifthVerticalEighthLine - mediumKnobDeltaX, topLine, mediumKnobWidth, mediumKnobHeight);
    postTrebleLabel.setBounds(fifthVerticalEighthLine - labelDeltaX, firstHorizontalSixthLine + mediumKnobLabelDeltaY, labelWidth, labelHeight);

    bypassSwitch.setBounds(seventhVerticalEighthLine - switchDeltaX, lastHorizontalSixthLine - switchDeltaY + verticalMargin, switchWidth, switchHeight);
    changeStageSwitch.setBounds(firstVerticalEighthLine - switchDeltaX, lastHorizontalSixthLine - switchDeltaY + verticalMargin, switchWidth, switchHeight);

    stageLED.setBounds(firstVerticalEighthLine - ledDeltaX, fourthHorizontalSixthLine, ledWidth, ledHeight);
    gainLED.setBounds(seventhVerticalEighthLine - ledDeltaX, fourthHorizontalSixthLine, ledWidth, ledHeight);
}

