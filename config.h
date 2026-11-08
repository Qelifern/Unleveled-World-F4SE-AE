#pragma once
#include "f4se_common/f4se_version.h" // F4SE
#include "IVersionCheck.h" // Common plugin version check, found in ..\plugins_common\
// #include "../plugin_common/common.h" // EG: Common plugin functions for this sdk if needed

// ================================
// Plugin Information
// ================================

// Plugin info
#define PLUGIN_AUTHOR				"QELIFERN"						// Author of the plugin

#define PLUGIN_NAME_SHORT			"unleveled_world_ae"	// Plugin name without spaces - same as in the vcxproj
#define PLUGIN_NAME_LONG			"Unleveled World AE"	// Plugin name with spaces - shown in various places once loaded

// Plugin version, Major, Minor, Build. EG: (1.1.0) equals (Major.Minor.Build)
#define PLUGIN_VERSION_MAJOR		1
#define PLUGIN_VERSION_MINOR		1
#define PLUGIN_VERSION_BUILD		0

// ================================
// Nothing below this needs changing between plugins except maybe MINIMUM_RUNTIME_VERSION and/or SUPPORTED_RUNTIME_VERSION_STRICT
// ================================

// Plugin log file
#define PLUGIN_LOG_FILE				"\\My Games\\" GAME_SAVE_FOLDER_NAME("Fallout4") "\\F4SE\\" PLUGIN_LOG_FILE_NAME(PLUGIN_NAME_SHORT) // plugin log file location

// Common definitions
#define SDK_VERSION_STRING			VERSION_TO_STRING(F4SE_VERSION_INTEGER, F4SE_VERSION_INTEGER_MINOR, F4SE_VERSION_INTEGER_BETA)
#define PLUGIN_VERSION_STRING		VERSION_TO_STRING(PLUGIN_VERSION_MAJOR, PLUGIN_VERSION_MINOR, PLUGIN_VERSION_BUILD)

#define PLUGIN_VERSION_DLL			MAKE_EXE_VERSION(PLUGIN_VERSION_MAJOR, PLUGIN_VERSION_MINOR, PLUGIN_VERSION_BUILD)
#define PLUGIN_VERSION_INFO			PLUGIN_NAME_SHORT " v%s (0x%08X) (SDK v%s)", PLUGIN_VERSION_STRING, PLUGIN_VERSION_DLL, SDK_VERSION_STRING

// ================================
// Plugin Version Information
// ================================

// The minimum runtime version allowed by the plugin. EG: I need function X, which was added in runtime Y, therefore Y is the minimum runtime version
#define MINIMUM_RUNTIME_VERSION				RUNTIME_VERSION_1_11_240 // The lowest next generation version

// The runtime version the plugin was designed for
// #define SUPPORTED_RUNTIME_VERSION		CURRENT_RELEASE_RUNTIME
#define SUPPORTED_RUNTIME_VERSION			RUNTIME_VERSION // Set in /config/_Plugins.props, controls core and plugin versions - normally latest

// Strict runtime version. If strict=true, runtime and supported versions must match. If strict=false, runtime version must be above the minimum
#define SUPPORTED_RUNTIME_VERSION_STRICT	true

// The compatible runtime version - set automatically
#define COMPATIBLE(runtimeVersion)			(runtimeVersion == SUPPORTED_RUNTIME_VERSION)

// ================================
