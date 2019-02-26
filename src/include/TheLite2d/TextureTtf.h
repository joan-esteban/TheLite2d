#pragma once
#include <string>
#include <SDL_ttf.h>
#include "TheLite2d/TtfFont.h"
#include "TheLite2d/SDLWrapper.h"
#include "TheLite2d/Texture.h"
#include "TheLite2d/IDrawable.h"
#include "TheLite2d/Texture.h"
#include "TheLite2d/log.h"

namespace thelite2d {
	class TextureTtf : public Texture {
	public:
		TextureTtf(SDLWrapper& sdl) :  Texture{ sdl }, sdl{ sdl } {};
		TextureTtf(SDLWrapper& sdl, std::shared_ptr<TtfFont> &newFont, const std::string &newMsg, const SDL_Color &newColor) : sdl{ sdl }, Texture{ sdl } {
            SPDLOG_INFO(getLogger(),"TextureTtf msg={0} newFont {1}", newMsg);
            setFont(newFont);
			setText(newMsg);
			setColor(newColor);
		};
		~TextureTtf() = default;

		

		void setFont(std::shared_ptr<TtfFont> &newFont) {
			if (font == newFont) return;
			font = newFont;
			needRegenerate = true;
		}
		void setText(const std::string &newMsg) {
			if (msg == newMsg) return;
			msg = newMsg;
			needRegenerate = true;
		}

		void setColor(const SDL_Color &newColor) {
			//if (color == newColor) return;
			color = newColor; 
			needRegenerate = true;
		}

		virtual int getWidth() const override {
			const_cast<TextureTtf *>(this)->generateTextureIfNeeded();
			return Texture::getWidth();
		};
		virtual int getHeight() const override{
			const_cast<TextureTtf *>(this)->generateTextureIfNeeded();
			return Texture::getHeight();
		};

		

		virtual std::weak_ptr<SDL_Texture> getSdltexture()  {
			generateTextureIfNeeded();
			return Texture::getSdltexture();
		}

		static Texture createInstance(SDLWrapper& sdl, std::shared_ptr<TtfFont> &newFont, const std::string &newMsg, const SDL_Color &newColor) {
			auto text =  TextureTtf{ sdl, newFont , newMsg, newColor };
			// TODO: ugly!
			text.forceGenerateTexture();
			return text;
		}

	protected:
		void generateTextureIfNeeded() {
            //SPDLOG_INFO(getLogger(),"generateTextureIfNeeded {0}",needRegenerate);
			if (needRegenerate && forceGenerateTexture() ) needRegenerate = false;
		}

	

		bool forceGenerateTexture() {
            //SPDLOG_INFO(getLogger(),"forceGenerateTexture");
			auto textSurface = SdlSurfaceUniquePtr(TTF_RenderText_Blended(font->getFont(), msg.c_str(), color), SDL_FreeSurface);
			if (textSurface == nullptr) {
                SPDLOG_ERROR(getLogger(),"forceGenerateTexture FAILS");
				return false;
			};
			return convertSurfaceToTexture(textSurface);
		}
		SDLWrapper& sdl;
		std::shared_ptr<TtfFont> font;
		std::string msg;
		SDL_Color color;
		bool needRegenerate = true;
	};
}

