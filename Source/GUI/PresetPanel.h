#pragma once

#include <JuceHeader.h>

#include "../Service/PresetManager.h"

namespace GUI
{
	class PresetPanel : public juce::Component, juce::Button::Listener, juce::ComboBox::Listener
	{
	public:
		PresetPanel(Service::PresetManager& pm);
		~PresetPanel();
		
		void resized() override;

		
	private:
		void buttonClicked(juce::Button* button) override;

		void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;

		void buttonConfiguration(juce::ImageButton& button, const juce::Image& normalImage, const juce::Image& downImage);

		void loadPresetList();

		juce::ImageButton savePresetButton, previousPresetButton, nextPresetButton;
		juce::ComboBox presetList;

		juce::Image previousNormalImage, previousDownImage, 
					nextNormalImage, nextDownImage, 
					saveNormalImage, saveDownImage;

		Service::PresetManager presetManager;

		std::unique_ptr< juce::FileChooser> fileChooser;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetPanel)
	};
}