#pragma once

#include <JuceHeader.h>

namespace Service
{
	class PresetManager : juce::ValueTree::Listener
	{
	public:
		PresetManager(juce::AudioProcessorValueTreeState&);

		void savePreset(const juce::String& presetName);
		void loadPreset(const juce::String& presetName);

		juce::StringArray getAllPresets();
		juce::String getCurrentPreset();

		int loadNextPreset();
		int loadPreviousPreset();

		static const juce::File defaultPresetFolder;
		static const juce::String presetExtension;
		static const juce::String presetNameProperty;

	private:
		void valueTreeRedirected(juce::ValueTree& tree) override;

		juce::AudioProcessorValueTreeState& valueTreeState;
		juce::Value currentPreset;
	};
}