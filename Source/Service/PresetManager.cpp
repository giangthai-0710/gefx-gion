#include "PresetManager.h"


const juce::File Service::PresetManager::defaultPresetFolder 
{ 
	juce::File::getSpecialLocation(juce::File::userDocumentsDirectory).getChildFile(ProjectInfo::companyName)
																	  .getChildFile(ProjectInfo::projectName)
																	  .getChildFile("Presets") 
};

const juce::String Service::PresetManager::presetExtension{ ".preset" };

const juce::String Service::PresetManager::presetNameProperty{ "presetName" };

Service::PresetManager::PresetManager(juce::AudioProcessorValueTreeState& aptvs) :
	valueTreeState(aptvs)
{
	// Create default preset folder
	if (!defaultPresetFolder.exists())
	{
		const auto result = defaultPresetFolder.createDirectory();
		if (!result.wasOk())
		{
			DBG("Failed to create default preset folder: " + result.getErrorMessage());
			jassertfalse;
		}
	}

	valueTreeState.state.addListener(this);
	currentPreset.referTo(valueTreeState.state.getPropertyAsValue(presetNameProperty, nullptr));

}

void Service::PresetManager::savePreset(const juce::String& presetName)
{
	if (presetName.isEmpty())
	{
		DBG("Preset name is empty");
		return;
	}

	currentPreset.setValue(presetName);
	const auto xml = valueTreeState.copyState().createXml();
	const auto presetFile = defaultPresetFolder.getChildFile(presetName + presetExtension);
	if (!xml->writeTo(presetFile))
	{
		DBG("Failed to write preset to file: " + presetFile.getFullPathName());
		jassertfalse;
	}
}

void Service::PresetManager::loadPreset(const juce::String& presetName)
{
	if (presetName.isEmpty())
	{
		return;
	}

	const auto presetFile = defaultPresetFolder.getChildFile(presetName + presetExtension);
	if (!presetFile.existsAsFile())
	{
		DBG("Preset file does not exist: " + presetFile.getFullPathName());
		jassertfalse;
		return;
	}

	juce::XmlDocument xmlDocument{ presetFile };
	const auto valueTreeToLoad = juce::ValueTree::fromXml(*xmlDocument.getDocumentElement());
	
	valueTreeState.replaceState(valueTreeToLoad);
	currentPreset.setValue(presetName);
}

juce::StringArray Service::PresetManager::getAllPresets()
{
	juce::StringArray presets;

	const auto fileArray = defaultPresetFolder.findChildFiles(juce::File::TypesOfFileToFind::findFiles, false, "*" + presetExtension);

	for (const auto& file : fileArray)
	{
		presets.add(file.getFileNameWithoutExtension());
	}

	return presets;
}

juce::String Service::PresetManager::getCurrentPreset()
{
	return currentPreset.toString();
}

int Service::PresetManager::loadNextPreset()
{
	const auto allPresets = getAllPresets();

	if (allPresets.isEmpty())
	{
		DBG("No presets found");
		return -1;
	}

	const auto currentPresetIndex = allPresets.indexOf(currentPreset.toString());
	const auto nextPresetIndex = (currentPresetIndex + 1 > allPresets.size() - 1) ? 0 : currentPresetIndex + 1;

	loadPreset(allPresets[nextPresetIndex]);

	return nextPresetIndex;
}

int Service::PresetManager::loadPreviousPreset()
{
	const auto allPresets = getAllPresets();

	if (allPresets.isEmpty())
	{
		DBG("No presets found");
		return -1;
	}

	const auto currentPresetIndex = allPresets.indexOf(currentPreset.toString());
	const auto previousPresetIndex = (currentPresetIndex - 1 < 0) ? allPresets.size() - 1 : currentPresetIndex - 1;

	loadPreset(allPresets[previousPresetIndex]);

	return previousPresetIndex;
}

void Service::PresetManager::valueTreeRedirected(juce::ValueTree& tree)
{
	currentPreset.referTo(tree.getPropertyAsValue(presetNameProperty, nullptr));
}
