// User Defines
#include "config.h"
// F4SE
#include "f4se/GameAPI.h"
#include "f4se/PluginAPI.h"
#include "f4se/PapyrusNativeFunctions.h"
#include "f4se_common/SafeWrite.h"

#include "f4se/GameAPI.h"
#include "f4se/GameForms.h"
#include "f4se/GameData.h"
#include <GameRTTI.h>

#include "f4se/PapyrusVM.h"
#include "f4se/PapyrusNativeFunctions.h"


#include "SimpleIni.h"

// Windows
#include <shlobj.h>	// CSIDL_MYCODUMENTS



bool unlevelItems = false;
bool unlevelCharacters = false;
CSimpleIniA ini(true, false, false);


bool GetOptionValueBool(const char* a_optionValue)
{
	std::string lowercaseStr;

	size_t length = std::strlen(a_optionValue);

	for (size_t i = 0; i < length; ++i) {
		lowercaseStr += static_cast<char>(std::tolower(static_cast<unsigned char>(a_optionValue[i])));
	}

	if (lowercaseStr == "true") {
		return true;
	}
	else {
		return false;
	}
}

void LoadConfigs()
{
	ini.LoadFile("Data\\F4SE\\Plugins\\unleveled_world_ae.ini");

	unlevelItems = GetOptionValueBool(ini.GetValue("General", "UnlevelItems", "false"));
	unlevelCharacters = GetOptionValueBool(ini.GetValue("General", "UnlevelCharacters", "false"));

	ini.Reset();

	_MESSAGE("Config file loaded - unleveled_world_ae.ini");

}



void UnlevelStuff()
{
	

	DataHandler* dataHandler = (*g_dataHandler);

	if (!dataHandler) {
		_MESSAGE("Error: g_dataHandler pointer is null.");
		return;
	}
	

	if (unlevelItems) {
		_MESSAGE("Unleveling item lists");
		size_t alteredEntriesCount = 0;

		auto& formArray = dataHandler->arrLVLI;

		for (uint32_t i = 0; i < formArray.count; ++i) {
			TESForm* form = formArray.entries[i];
			if (!form) continue;

			TESLeveledList* levList = DYNAMIC_CAST(form, TESForm, TESLeveledList);
			if (levList) {
				TESLeveledList::Entry* entries = levList->entries;

				uint8_t numEntries = levList->length;

				if (entries && numEntries > 0) {
					for (uint8_t j = 0; j < numEntries; ++j) {
						if (entries[j].level > 1) {
							entries[j].level = 1;
							alteredEntriesCount++;
						}
					}
				}
			}
		}
		_MESSAGE("Altered %zu entries in item lists.", alteredEntriesCount);
	}
	else {
		_MESSAGE("Ignoring item lists");
		_MESSAGE("Disabled Unleveling Item Lists - Config file: unleveled_world_ae.ini");
	}

	if (unlevelCharacters) {
		_MESSAGE("Unleveling character lists");
		size_t alteredEntriesCount = 0;

		auto& formArray = dataHandler->arrLVLN;

		
		for (uint32_t i = 0; i < formArray.count; ++i) {
			TESForm* form = formArray.entries[i];
			if (!form) continue;

			TESLeveledList* levList = DYNAMIC_CAST(form, TESForm, TESLeveledList);
			if (levList) {
				TESLeveledList::Entry* entries = levList->entries;

				uint8_t numEntries = levList->length;

				

				if (entries && numEntries > 0) {
					for (uint8_t j = 0; j < numEntries; ++j) {
						if (entries[j].level > 1) {
							entries[j].level = 1;
							alteredEntriesCount++;
						}
					}
				}
			}
		}
		_MESSAGE("Altered %zu entries in character lists.", alteredEntriesCount);
	}
	else {
		_MESSAGE("Ignoring character lists");
		_MESSAGE("Disabled Unleveling Character Lists - Config file: unleveled_world_ae.ini");
	}
	
}

void OnF4SEMessage(F4SEMessagingInterface::Message* msg) {
	


	switch (msg->type) {
		case F4SEMessagingInterface::kMessage_PostLoadGame:
		{
			_MESSAGE("PluginListener: kMessage_PostLoadGame");
			UnlevelStuff();
			break;
		}
		case F4SEMessagingInterface::kMessage_NewGame:
		{
			_MESSAGE("PluginListener: kMessage_NewGame");
			UnlevelStuff();
			break;
		}
		case F4SEMessagingInterface::kMessage_GameDataReady:
		{
			_MESSAGE("PluginListener: kMessage_GameDataReady");
			UnlevelStuff();
			break;
		}
	}
}



// ================================
// Handles
// ================================

// Handle for the plugin
PluginHandle g_pluginHandle = kPluginHandle_Invalid;

// Handle for the papyrus interface
static F4SEPapyrusInterface* g_papyrus = NULL;

// Handle for the messaging interface
static F4SEMessagingInterface* g_messaging = NULL;


// ================================
// Plugin Compatibility Check
// ================================

// Check that the runtime and plugin versions are compatible
const bool IsCompatible(const F4SEInterface* f4se)
{
	// disable the plugin if it's in the editor
	if (f4se->isEditor) {
		_MESSAGE("ERROR::IsCompatible: Plugin is not compatible with the editor, disabling");
		_FATALERROR("ERROR::IsCompatible: Plugin is not compatible with the editor, disabling");
		return false;
	}
	// check for compatible version. if strict=true, runtime and supported versions must match. if strict=false, runtime version must be above the minimum
	else if (!IVersionCheck::IsCompatibleVersion(f4se->runtimeVersion, MINIMUM_RUNTIME_VERSION, SUPPORTED_RUNTIME_VERSION, SUPPORTED_RUNTIME_VERSION_STRICT)) { // not strict
		_MESSAGE("ERROR::IsCompatible: Plugin is not compatible with runtime version, disabling");
		_FATALERROR("ERROR::IsCompatible: Plugin is not compatible with runtime version, disabling");
		
		// not compatible
		return false;
	}
	
	// compatible
	return true;
}

// ================================
// Plugin Export, Query and Load - Start Extern-C
// ================================

extern "C" { // Start: extern "C"

// ================================
// Plugin Version Data - Not Used In All SE SDK's
// ================================

// Export the plugin version data to f4se, without this the plugin will fail to load. See: PluginAPI.h
__declspec(dllexport) F4SEPluginVersionData F4SEPlugin_Version =
{
	F4SEPluginVersionData::kVersion, // f4se expects plugin version info to follow this structure
	
	PLUGIN_VERSION_DLL,		// set in config.h
	PLUGIN_NAME_LONG, 		// set in config.h
	PLUGIN_AUTHOR, 			// set in config.h

	0,	// not version independent
	0,	// not version independent (extended field)
	
	// { RUNTIME_VERSION_1_10_980, 0 },	// compatible with 1.10.980 // NextGen
	// { RUNTIME_VERSION_1_10_984, 0 }, // compatible with NextGen plus Hotfix, See: f4se_version.h
	{ SUPPORTED_RUNTIME_VERSION, 0 }, // set in config.h
	
	0,	// works with any version of the script extender. you probably do not need to put anything here
	
	// 0, 0, 0
};

// ================================
// Plugin Query - Not Used In All SE SDK's
// ================================

// F4SE Plugin Query - Called when the plugin is queried
bool F4SEPlugin_Query(const F4SEInterface* f4se, PluginInfo* info)
{
	// open the plugin log file
	gLog.OpenRelative(CSIDL_MYDOCUMENTS, PLUGIN_LOG_FILE);
	
	_MESSAGE(PLUGIN_VERSION_INFO); // write the plugin version to the log
	_MESSAGE("Plugin_Query: Querying");

	// populate info structure
	info->infoVersion =	PluginInfo::kInfoVersion;
	info->name =		PLUGIN_NAME_LONG;		// set in config.h
	info->version =		PLUGIN_VERSION_DLL;		// set in config.h
	
	// check for compatible versions
	if (!IsCompatible(f4se)) {
		_MESSAGE("ERROR::Plugin_Query: Incompatible | Disabling Plugin");
		_FATALERROR("ERROR::Plugin_Query: Incompatible | Disabling Plugin");
		// incompatible
		return false;
	}
	
	// ######################################################
	// Note: Nothing should be assigned below this point
	// Get the necessary info about the plugin and return
	// ######################################################
	
	_MESSAGE("Plugin_Query: Queried Successfully"); // log the successful query

	// supported runtime version
	return true;
}

// ================================
// Plugin Load - Used In All SE SDK's
// ================================

// F4SE Plugin Load - Called when the plugin loads everytime
bool F4SEPlugin_Load(const F4SEInterface* f4se)
{
	gLog.OpenRelative(CSIDL_MYDOCUMENTS, PLUGIN_LOG_FILE);

	_MESSAGE(PLUGIN_VERSION_INFO); 
	_MESSAGE("Plugin_Load: Loading");
	
	if (!IsCompatible(f4se)) {
		_MESSAGE("ERROR::Plugin_Load: Incompatible | Disabling Plugin");
		_FATALERROR("ERROR::Plugin_Load: Incompatible | Disabling Plugin");
		return false;
	}

	g_pluginHandle = f4se->GetPluginHandle();
	
	g_papyrus = (F4SEPapyrusInterface *)f4se->QueryInterface(kInterface_Papyrus);
	if (!g_papyrus) {
		_MESSAGE("ERROR::Plugin_Load: Could not get the papyrus interface, disabling");
		_FATALERROR("ERROR::Plugin_Load: Could not get the papyrus interface, disabling");
		return false;
	}


	_MESSAGE("Plugin loaded into memory.");
	F4SEMessagingInterface* messaging = (F4SEMessagingInterface*)f4se->QueryInterface(kInterface_Messaging);

	if (messaging) {
		// Tell F4SE to send game events to your OnF4SEMessage function
		messaging->RegisterListener(f4se->GetPluginHandle(), "F4SE", OnF4SEMessage);
		_MESSAGE("Registered messaging listener successfully.");
	}
	else {
		_MESSAGE("Warning: Could not get messaging interface. Mod will not run.");
		return false;
	}

	LoadConfigs();
	
	_MESSAGE("Plugin_Load: Loaded Successfully"); // log the successful load

	// initialized
	return true;
}

}; // Stop: extern "C"

// ================================
