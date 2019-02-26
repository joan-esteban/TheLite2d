#include <sstream>
#include <cassert>
#include "TheLite2d/Mouse.h"
#include "TheLite2d/IPositionable.h"
#include "TheLite2d/config.h"
#include "TheLite2d/TtfFont.h"
#include "TheLite2d/log.h"

thelite2d::TtfFont::TtfFont(const std::string &fullFilenameTtf, int size, TtfFontCache *cache) {
	bool res = loadFromFile(fullFilenameTtf, size);
	assert(res == true);
}

bool thelite2d::TtfFont::loadFromFile(const std::string &fullFilenameTtf, int size, TtfFontCache *cache) {
	SPDLOG_INFO(getLogger(), "Loading font  [{0}] size={1}", fullFilenameTtf.c_str(), size);
	//std::string filename = thelite2d::config::pathAssets + fullFilenameTtf;
	std::string filename = fullFilenameTtf;

	if (cache && cache->exists({ fullFilenameTtf, size })) {
		font = cache->get({ fullFilenameTtf, size });
		return true;
	};
	auto rawPointerFont = TTF_OpenFont(filename.c_str(), size);
	if (!rawPointerFont) {
		SPDLOG_ERROR(getLogger(), "Error loading font [{0}] expeted file [{1}] SDL error: [{2}]",
			fullFilenameTtf.c_str(), filename.c_str(), SDL_GetError());
		return false;
	}
	font = std::shared_ptr<TTF_Font>(rawPointerFont, TTF_CloseFont);
	if (cache) cache->insert({ fullFilenameTtf, size }, font);
	SPDLOG_INFO(getLogger(), "Loaded font  [{0}]", filename.c_str());
	return true;
}

TTF_Font *thelite2d::TtfFont::getFont() const {
	return font.get();
}

std::shared_ptr<thelite2d::TtfFont> thelite2d::TtfFont::createInstance(const std::string &fullFilenameTtf, int size, TtfFontCache *cache) {
	return std::shared_ptr<TtfFont>(new thelite2d::TtfFont{ fullFilenameTtf , size, cache });
}