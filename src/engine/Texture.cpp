#include <cassert>
#include <memory>
#include <sstream>
#include <SDL_image.h>

#include "TheLite2d/log.h"
#include "TheLite2d/Texture.h"
#include "TheLite2d/config.h"

using namespace thelite2d;

Texture::Texture(SDLWrapper &sdl) : sdl{&sdl}{
}
Texture::Texture(SDLWrapper &sdl, const std::string &fullFilename, TextureCache *cache) : sdl{ &sdl } {
	bool loaded = loadFromFile(fullFilename, cache);
	assert(loaded);
}
Texture::Texture(SDLWrapper &sdl, std::shared_ptr<SDL_Texture> sdlTexture, SDL_Rect &sourceRect) : sdl{ &sdl },
		texture{ std::move(sdlTexture),sourceRect  }  {

}
Texture::Texture(SDLWrapper &sdl, Dimensions dimensions, SDL_Color &color, TextureCache *cache) :
	Texture{ sdl, convertFloatToInt(dimensions.x), convertFloatToInt(dimensions.y) , color, cache } {};

Texture::Texture(SDLWrapper &sdl, int w, int h, SDL_Color &color, TextureCache *cache) : sdl{ &sdl } {
	char buf[1024];
	snprintf(buf, sizeof(buf), "solid:%03d-%03d_color-%03d.%03d.%03d.%03d", w, h, color.r, color.g, color.b, color.a);
	buf[sizeof(buf) - 1] = 0;
	std::string cacheKey = buf;
	if (cache && cache->exists(cacheKey)) {
		texture = cache->get(cacheKey);
		return;
	}
	auto render = sdl.getSdlRenderer();
	SDL_Texture *textureRawPointer = SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET, w, h);
	SDL_SetTextureBlendMode(textureRawPointer, SDL_BLENDMODE_BLEND);
	auto sharedTexture = std::shared_ptr<SDL_Texture>(textureRawPointer, SDL_DestroyTexture);
	SDL_SetRenderTarget(render, textureRawPointer);
	SDL_SetRenderDrawColor(render, color.r, color.g, color.b, color.a);
	SDL_RenderClear(render);
	SDL_SetRenderTarget(render, NULL);
	texture = { sharedTexture, w, h };
	if (cache) cache->insert(cacheKey, texture);

}

bool Texture::loadFromFile(std::string fullFilename, thelite2d::utils::Cache<std::string, TextureData> *cache) {
    SPDLOG_INFO(getLogger(),"Loading image {0}]", fullFilename.c_str());
	std::string filename = fullFilename;
	if (cache && cache->exists(filename)) {
		texture = cache->get(filename);
		return true;
	};
	// TODO: This must be a unique_ptr
	//auto loadedSurface = std::unique_ptr<SDL_Surface>(IMG_Load(filename.c_str()), SDL_FreeSurface);
	auto loadedSurface = SdlSurfaceUniquePtr{ IMG_Load(filename.c_str()), SDL_FreeSurface };

    if (!loadedSurface) {
        SPDLOG_ERROR(getLogger(),"Error loading image [{0}] SDL error: [{1}]", fullFilename.c_str(), SDL_GetError() );
        return false;
    }
    SPDLOG_TRACE(getLogger(),"Loaded image [{0}]", fullFilename.c_str());
	if (convertSurfaceToTexture(loadedSurface )) {
		if (cache) cache->insert(filename, texture);
		return true;
	}
	else {
		SPDLOG_ERROR(getLogger(), "Loaded image [{0}] error convertint to surface", fullFilename.c_str());
		return false;
	}
}

bool Texture::convertSurfaceToTexture(SdlSurfaceUniquePtr &surface) {
	if (!surface) return false;
	if (!sdl) return false;
	SDL_Texture *textureRawPointer = SDL_CreateTextureFromSurface(sdl->getSdlRenderer(), surface.get());
	auto texturePointer = std::shared_ptr<SDL_Texture>(textureRawPointer, SDL_DestroyTexture);
	texture = { texturePointer, surface->w, surface->h };
	return (textureRawPointer != nullptr);
}
