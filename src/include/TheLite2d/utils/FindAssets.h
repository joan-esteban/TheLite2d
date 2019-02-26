#pragma once
#include <string>
#include <list>
#include "TheLite2d/utils/Files.h"

namespace thelite2d {
namespace utils {
	/// This object find a asset from a search path
	class FindAssets {
	public:
		/// Use default search path  thelite2d::config::searchPathAssets
		FindAssets();

		/// Set a custom search path
		FindAssets(std::list<std::string> searchPath);

		/// Returns full filename for a asset or filename
		std::string get(const std::string &filename) const;

		const std::list<std::string> &getSearchPath() const;
	private:
		bool useGlobalSearchPathAssets;
		std::list<std::string> searchPath;
		Files filesSupport;
	};
}
}