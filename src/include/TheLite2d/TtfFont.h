#pragma once
#include <string>
#include <memory>
#include <cassert>
#include <SDL.h>
#include <SDL_ttf.h>
#include "log.h"
#include "utils/cache.h"
#include "config.h"

namespace thelite2d {
	class TtfFont;

	struct TtfFontCacheData {
		int size;
		std::shared_ptr<TTF_Font> ttfFont = nullptr;
	};
	struct TtfFontId {
		std::string filename;
		int size;
		bool operator< (const TtfFontId& rsh) const {
			if (filename < rsh.filename) return true;
			if (filename > rsh.filename) return false;
			return (size < rsh.size);
		};
	};
	using TtfFontCache = thelite2d::utils::Cache< TtfFontId, std::shared_ptr<TTF_Font> >;
	class TtfFont {
	public:
		TtfFont() = default;
		TtfFont(const std::string &fullFilenameTtf, int size, TtfFontCache *cache = nullptr);
		virtual ~TtfFont() = default;

		bool loadFromFile(const std::string &fullFilenameTtf, int size, TtfFontCache *cache = nullptr);

		TTF_Font *getFont() const;

		static std::shared_ptr<TtfFont> createInstance(const std::string &fullFilenameTtf, int size, TtfFontCache *cache = nullptr);

	private:
		std::shared_ptr<TTF_Font> font = nullptr;
	};
}