#pragma once
#include <list>
#include "TheLite2d/types.h"

//#define FIT_ON_SCREEN

namespace thelite2d {
	namespace config {
		
#ifndef __ANDROID__
		// windowsSize in windowed execution 
		static const Dimensions windowsSize = Dimensions{ 1280.0, 720.0 } / 2;
		static const std::string defaultPathAssets = { "../../assets/" };
#else // ANDROID
		static const std::string defaultPathAssets = {};	// Assets doesnt have prefixs
#endif 
		static const bool defaultDebugMode = false;
		
		extern  bool debugMode;
		extern  std::list<std::string> searchPathAssets;
	}
}