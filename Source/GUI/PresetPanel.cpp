#include "PresetPanel.h"

GUI::PresetPanel::PresetPanel(Service::PresetManager& pm) : presetManager(pm)
{
	setWantsKeyboardFocus(true);

	previousNormalImage = juce::ImageCache::getFromMemory(BinaryData::Previous_button_normal_png, BinaryData::Previous_button_normal_pngSize);
	previousDownImage = juce::ImageCache::getFromMemory(BinaryData::Previous_button_clicked_png, BinaryData::Previous_button_clicked_pngSize);

	nextNormalImage = juce::ImageCache::getFromMemory(BinaryData::Next_button_normal_png, BinaryData::Next_button_normal_pngSize);
	nextDownImage = juce::ImageCache::getFromMemory(BinaryData::Next_button_clicked_png, BinaryData::Next_button_clicked_pngSize);

	saveNormalImage = juce::ImageCache::getFromMemory(BinaryData::Save_button_normal_png, BinaryData::Save_button_normal_pngSize);
	saveDownImage = juce::ImageCache::getFromMemory(BinaryData::Save_button_clicked_png, BinaryData::Save_button_clicked_pngSize);

	buttonConfiguration(previousPresetButton, previousNormalImage, previousDownImage);
	buttonConfiguration(nextPresetButton, nextNormalImage, nextDownImage);
	buttonConfiguration(savePresetButton, saveNormalImage, saveDownImage);

	presetList.setTextWhenNothingSelected("Presets");
	presetList.setColour(juce::ComboBox::backgroundColourId, juce::Colour(0xff1A1A1A));
	presetList.setColour(juce::ComboBox::outlineColourId, juce::Colour(0xff1A1A1A));
	presetList.setMouseCursor(juce::MouseCursor::PointingHandCursor);
	addAndMakeVisible(presetList);
	presetList.addListener(this);

	addKeyListener(this);

	loadPresetList();

}

GUI::PresetPanel::~PresetPanel()
{
	nextPresetButton.removeListener(this);
	previousPresetButton.removeListener(this);
	savePresetButton.removeListener(this);

	presetList.removeListener(this);

	removeKeyListener(this);
}

void GUI::PresetPanel::resized()
{
	auto width = getWidth();
	auto height = getHeight();

	auto padding = 10;

	auto buttonWidth = 20;
	auto buttonHeight = 20;

	auto comboBoxWidth = 100;
	auto comboBoxHeight = 20;

	savePresetButton.setBounds(width - buttonWidth - padding, 5, buttonWidth, buttonHeight);

	nextPresetButton.setBounds((width + comboBoxWidth) / 2 + padding, 5, buttonWidth, buttonHeight);
	previousPresetButton.setBounds((width - comboBoxWidth) / 2 - padding - buttonWidth, 5, buttonWidth, buttonHeight);

	presetList.setBounds((width - comboBoxWidth) / 2, 5, comboBoxWidth, comboBoxHeight);

}

void GUI::PresetPanel::buttonClicked(juce::Button* button)
{
	if (button == &savePresetButton)
	{
		fileChooser = std::make_unique<juce::FileChooser>("Save Preset",
														  Service::PresetManager::defaultPresetFolder,
														  "*" + Service::PresetManager::presetExtension);
		fileChooser->launchAsync(juce::FileBrowserComponent::saveMode, [&](const juce::FileChooser& chooser)
		{
			const auto resultFile = chooser.getResult();
			presetManager.savePreset(resultFile.getFileNameWithoutExtension());
			loadPresetList();
		});
	}
	else if (button == &nextPresetButton)
	{
		// Load next preset
		const auto index = presetManager.loadNextPreset();
		presetList.setSelectedItemIndex(index, juce::dontSendNotification);
	}
	else if (button == &previousPresetButton)
	{
		// Load previous preset
		const auto index = presetManager.loadPreviousPreset();
		presetList.setSelectedItemIndex(index, juce::dontSendNotification);
	}
}

void GUI::PresetPanel::comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged)
{
	if (comboBoxThatHasChanged == &presetList)
	{
		presetManager.loadPreset(presetList.getItemText(presetList.getSelectedItemIndex()));
	}
}

void GUI::PresetPanel::buttonConfiguration(juce::ImageButton& button, const juce::Image& normalImage, const juce::Image& downImage)
{
	button.setImages(true, true, true, normalImage, 1.0f, {}, {}, 1.0f, {}, downImage, 1.0f, {}, {});
	button.setMouseCursor(juce::MouseCursor::PointingHandCursor);
	addAndMakeVisible(button);
	button.addListener(this);
}

bool GUI::PresetPanel::keyPressed(const juce::KeyPress& key, juce::Component* originatingComponent)
{
	if (key == juce::KeyPress::rightKey)
	{
		DBG("Right key pressed");
		nextPresetButton.triggerClick();
		
	}
	else if (key == juce::KeyPress::leftKey)
	{
		DBG("Left key pressed");
		previousPresetButton.triggerClick();
	}
	return false;
}

void GUI::PresetPanel::loadPresetList()
{
	presetList.clear(juce::dontSendNotification);
	const auto allPresets = presetManager.getAllPresets();
	const auto currentPreset = presetManager.getCurrentPreset();
	presetList.addItemList(allPresets, 1);
	presetList.setSelectedItemIndex(allPresets.indexOf(currentPreset), juce::dontSendNotification);
}
