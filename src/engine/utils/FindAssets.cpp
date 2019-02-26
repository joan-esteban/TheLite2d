#include "TheLite2d/utils/FindAssets.h"
#include "TheLite2d/config.h"
#include <stdio.h>

thelite2d::utils::FindAssets::FindAssets() : useGlobalSearchPathAssets{ true } {

}

thelite2d::utils::FindAssets::FindAssets(std::list<std::string> searchPath) :
		searchPath{ std::move(searchPath) }, 
		useGlobalSearchPathAssets{ false } { 
}

std::string thelite2d::utils::FindAssets::get(const std::string &filename) const {
	const std::list<std::string> &search = getSearchPath();
	for (auto path : search) {
		std::string proposedFilename = path + "/" + filename;
		if (filesSupport.fileExists(proposedFilename)) return proposedFilename;
	}
	return filename;
}

const std::list<std::string> &thelite2d::utils::FindAssets::getSearchPath() const {
	if (useGlobalSearchPathAssets) return thelite2d::config::searchPathAssets;
	return searchPath;
}
