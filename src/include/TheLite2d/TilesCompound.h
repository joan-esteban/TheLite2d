#pragma once
#include <list>
#include <vector>
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
#include "impl/Insertable.h"
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
	class TilesCompound :public Object, public thelite2d::impl::Controlable, public thelite2d::impl::Positionable, public IDrawable, public thelite2d::impl::Insertable {
	public:
		TilesCompound(Texture texture) : thelite2d::impl::Controlable{ this } {
			textures.push_back(texture);
		};
		/*
		TilesCompound(std::initializer_list< std::shared_ptr<Texture > > lst) {
			textures.insert(textures.end(), lst.begin(), lst.end());
		};
		*/

		void setTilesDimesions(int w, int h) {
			tilesWidth = w; 
			tilesHeight = h;
		}

		virtual Dimensions getDimensions() const  override;

		// Heredado vía IDrawable
		virtual bool getDrawCommands(const Present & present, std::list<std::unique_ptr<IDrawCommand>>& drawCommands) override;

		//Dimensions getDimensions() const override {
		//	return textures.at(0)->getDimensions();
		//}

		static std::shared_ptr<TilesCompound> createInstance(Texture texture) {
			return std::shared_ptr<TilesCompound>(new TilesCompound{ std::move(texture) });
		}

	private:
		std::vector< Texture > textures;
		int tilesWidth = 3;
		int tilesHeight = 3;
		
		
	};
}