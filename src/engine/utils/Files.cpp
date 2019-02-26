#include "TheLite2d/utils/Files.h"
#include <stdio.h>

bool thelite2d::utils::Files::fileExists(const std::string &filename) const {
	FILE *file = nullptr;
	fopen_s(&file, filename.c_str(), "r");
	if (file) {
		fclose(file);
		return true;
	}
	return false;
}

std::string thelite2d::utils::Files::basename(const std::string &filename) const {
	auto pos = filename.find_last_of("/");
	if (pos == std::string::npos) {
		pos = filename.find_last_of("\\");
		if (pos == std::string::npos) return filename;
	}
	return filename.substr(0, pos+1);
}