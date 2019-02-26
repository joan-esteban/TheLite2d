#pragma once
#include <string>

namespace thelite2d {
namespace utils {
	class Files {
	public:
		bool fileExists(const std::string &filename) const;

		std::string basename(const std::string &filename) const;
	};
}
}
