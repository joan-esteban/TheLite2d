#pragma once
#include <list>
#include <memory>
#include <sstream>

#include "Texture.h"
#include "types.h"
#include "IDrawable.h"
#include "IController.h"
#include "IControllable.h"
#include "Object.h"
#include "IPositionable.h"
#include "IDrawable.h"
#include "ITexturable.h"
#include "IContainer.h"
#include "log.h"
#include "impl/Controllable.h"
#include "impl/Positionable.h"
#include "impl/Frameable.h"
#include "TheLite2d/drawcmd/blit.h"
#include "TheLite2d/drawcmd/text.h"
#include "TheLite2d/drawcmd/rectangle.h"
#include "config.h"
// https://msdn.microsoft.com/en-us/library/6b3sy7ae.aspx
// c4250 : class1' : inherits 'class2::member' via dominance
#pragma warning( disable: 4250)  

namespace thelite2d {
	//! This is a texture with data
	// class ISprite : public virtual Object, public virtual IPositionable, virtual public IDrawable, virtual public IControllable, virtual public ITexturable 
	class Sprite :public Object, public thelite2d::impl::Controlable, public thelite2d::impl::Positionable, public thelite2d::impl::Frameable, public IDrawable, public ITexturable, public IInsertable {
	public:	
		

		Sprite(Texture texture) : texture{ std::move(texture) }, thelite2d::impl::Controlable{ this } {
			
		};

		//std::map<std::string, std::vector <Texture> > frames;
		//int currentFrame = 0;
		// True if there are something to draw
		bool getDrawCommands(const Present &present, std::list<std::unique_ptr<IDrawCommand>> &drawCommands) override {
			SPDLOG_DEBUG(getLogger(),"Sprite {0} getDrawCommands ", getObjectName());
			if (updateCurrentFrame(present)) {
				SPDLOG_DEBUG(getLogger(),"Sprite {0} getDrawCommands  update texture", getObjectName());
				auto t = getFrameTexture();
				if (t) texture = *t;
			}
			SPDLOG_DEBUG(getLogger(),"Sprite {0} executeController", getObjectName());
			executeController(present, *this);
			SPDLOG_DEBUG(getLogger(),"Sprite {0} executeController after", getObjectName());
			bool returnValue = false;
			if (!getVisible()) return false;
			if (texture.getSdltexture().expired() ) {
				SPDLOG_DEBUG(getLogger(), "Sprite getDrawCommands texture is expired");
				return false;
			}
			if (getAlpha() <= constants::ALPHA_MIN) return false;
			if (thelite2d::config::debugMode) {
				std::stringstream ss;
				ss << "SPRITE:" << getObjectName() << "pos:" << getPosition() << " alpha:" << getAlpha();
				//auto drawTextCommand = std::dynamic_pointer_cast<IDrawCommand> (std::make_shared<drawcmd::Text>(getFullData(), ss.str(), SDL_Color{ 64,64,0,255 }));
				//drawCommands.push_back(drawTextCommand);
				drawCommands.push_back(std::unique_ptr<IDrawCommand>(new drawcmd::Text{ getFullData(),ss.str(), SDL_Color{ 64,64,0,255 } }));
				//auto drawRectCommand = std::dynamic_pointer_cast<IDrawCommand> (std::make_shared<drawcmd::Rectangle>(getFullData()));
				//drawCommands.push_back(drawRectCommand);
				drawCommands.push_back(std::unique_ptr<IDrawCommand>(new drawcmd::Rectangle{ getFullData() }));

			}

			//auto drawCommand = std::dynamic_pointer_cast<IDrawCommand> (std::make_shared<drawcmd::Blit>(texture, getFullData()));
			//drawCommands.push_back(drawCommand);
			drawCommands.push_back(std::unique_ptr<IDrawCommand>(new drawcmd::Blit{ texture,getFullData() }));
			return true;
		}
		

		Dimensions getDimensions() const override {
			return getTexture().getDimensions();
		}


		const Texture &getTexture() const override { return texture; };
		void setTexture(Texture newTexture)override { texture = std::move(newTexture); };
		void setParent(IContainer *newParent) override {
			parent = newParent;
		}
		IContainer *getParent() const override {
			return parent;
		}
		static std::shared_ptr<Sprite> createInstance(Texture texture) {
			return std::shared_ptr<Sprite>(new Sprite{ std::move(texture) });
		}

		static std::shared_ptr<Sprite> createInstance(const Sprite &sprite) {
			return std::shared_ptr<Sprite>(new Sprite{ sprite });
		}

		static std::shared_ptr<Sprite> createInstance(const std::shared_ptr<Sprite> &sprite) {
			return std::shared_ptr<Sprite>(new Sprite{ *sprite.get() });
		}
	protected:
		Texture texture;
		IContainer * parent = nullptr;
		std::map<std::string, std::vector <Texture> > frames;
		std::string activeFrameSet = "";
		int currentFrame = 0;
		
	};
}
#pragma warning( default: 4250)