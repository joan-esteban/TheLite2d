#ifndef BGAME_TEXTURE_H
#define BGAME_TEXTURE_H

// Based on code Lazy Foo' Productions (2004-2015) http://lazyfoo.net/tutorials/SDL/

#include <SDL.h>
#include <string>
#include <memory>
#include "TheLite2d/SDLWrapper.h"
#include "TheLite2d/IDrawable.h"
#include "utils/cache.h"
#include "TheLite2d/log.h"
//#include "drawcmd/blit.h"

namespace thelite2d {
	struct TextureData {
		TextureData() {};
		
		TextureData(std::shared_ptr<SDL_Texture> texture, int w, int h) : texture{ texture }, width{ w }, height{ h } {};
		TextureData(std::shared_ptr<SDL_Texture> texture, SDL_Rect srcRect) : texture{ texture }, 
			width{ srcRect.w }, height{ srcRect.h }, srcRect{ srcRect }, useSource{ true }  {};
		~TextureData() = default;

		std::shared_ptr<SDL_Texture> texture = nullptr;
		int width = 0;
		int height = 0;
		bool useSource = false;
		SDL_Rect srcRect = { 0,0,0,0 };
	};

	
	using TextureCache = thelite2d::utils::Cache<std::string, TextureData>;
	using SdlSurfaceUniquePtr = std::unique_ptr<SDL_Surface, std::function< void(SDL_Surface *) > >;
    class Texture
    {
    public:
        Texture(SDLWrapper &sdl);
		Texture(const Texture &sdl) = default;
		Texture(Texture &&sdl) = default;
		Texture(SDLWrapper &sdl, const std::string &fullFilename, TextureCache *cache = nullptr);
		Texture(SDLWrapper &sdl, std::shared_ptr<SDL_Texture> sdlTexture, SDL_Rect &sourceRect);
		Texture(SDLWrapper &sdl,int w, int h, SDL_Color &color, TextureCache *cache = nullptr);
		Texture(SDLWrapper &sdl, Dimensions dimensions, SDL_Color &color, TextureCache *cache = nullptr);
		//Texture(SDLWrapper &sdl, std::unique_ptr<SDL_Surface> &surface);
        virtual ~Texture() = default;

		Texture &operator=(const Texture &texture) = default;
		Texture &operator=(Texture &&texture) = default;

        bool loadFromFile( std::string fullFilename, TextureCache *cache = nullptr);
		
		SDLWrapper *getSdlWrapper() const { return sdl; };

		virtual std::weak_ptr<SDL_Texture> getSdltexture() const { return texture.texture; };
		
		virtual const SDL_Rect *sourceSdlRect() const { if (texture.useSource) return &texture.srcRect; return nullptr; };
        //Gets image dimensions
        virtual int getWidth() const  {return texture.width;} ;
		virtual int getHeight() const   {return texture.height;};
		Dimensions getDimensions() const  { return { FloatingPointType(getWidth()) , FloatingPointType(getHeight()) }; };

		static Texture createInstance(SDLWrapper &sdl, const std::string &fullFilename, TextureCache *cache = nullptr) {
			return  Texture{ sdl, fullFilename, cache };
		}

		/*static Texture createInstance(SDLWrapper &sdl, int w, int h, SDL_Color &color, TextureCache *cache = nullptr) {
			return  Texture{ sdl, w,h,color, cache };
		}*/

		static Texture createInstance(SDLWrapper &sdl, int w, int h, SDL_Color color, TextureCache *cache = nullptr) {
			return  Texture{ sdl, w,h,color, cache };
		}
    protected:
		bool convertSurfaceToTexture(SdlSurfaceUniquePtr &surface);
        //The actual hardware texture
		TextureData texture = {};
		

        SDLWrapper *sdl;
    };
}


#endif //BGAME_TEXTURE_H
