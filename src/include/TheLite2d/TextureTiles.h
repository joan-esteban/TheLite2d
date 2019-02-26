#pragma once
#include <map>
#include "Texture.h"

namespace thelite2d {
	class TextureTilesExtractor {
	public:

		std::vector <Texture> extract(const Texture &texture, int columns, int rows) const {
			std::vector <Texture> result;
			auto tileW = texture.getWidth() / columns;
			auto tileH = texture.getHeight() / rows;
			for (int j = 0; j < rows; j++) 
			for (int i = 0; i < columns; i++){
					SDL_Rect rect;
					rect.x = tileW * i;
					rect.y = tileH * j;
					rect.w = tileW;
					rect.h = tileH;
					auto frame = Texture{ *texture.getSdlWrapper(),texture.getSdltexture().lock(), rect };
					result.push_back(Texture{ *texture.getSdlWrapper(),texture.getSdltexture().lock(), rect });
				}
			return result;
		}

	private:
		
	};

	class TextureMultiFileExtractor {
	public:

		std::vector <Texture> extract(SDLWrapper &sdl, std::string pattern, int firstFrame, int lastFrame, TextureCache *cache ) const {
			std::vector <Texture> result;

			for (int i = firstFrame; i <= lastFrame; i++) {
				char buf[1024];
				snprintf(buf, sizeof(buf), pattern.c_str(), i);
				
				result.push_back(Texture{ sdl, buf,cache });
			}
			return result;
		}

	};
}

