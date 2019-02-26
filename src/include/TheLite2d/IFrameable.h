#pragma once
#include <string>
#include <vector>
#include "Texture.h"

namespace thelite2d {


	using FrameId = int;
	const FrameId UNSET_FRAME = -1;
	// 0 - 20ms, 1-20ms
	struct FrameData {
		// TODO: Fix it!
		//FrameData(): texture{*dynamic_cast<SDLWrapper*>(0) }{};
		FrameData(const Texture &texture, FloatingPointType showDuringInSeconds, FrameId jumpToFrame ):
			texture{texture}, showDuringInSeconds{showDuringInSeconds},jumpToFrame{jumpToFrame} {};

		Texture texture;
		FloatingPointType showDuringInSeconds; 
		FrameId jumpToFrame = UNSET_FRAME;
	};

	class IFrameable {
	public:
		virtual ~IFrameable() = default;

		virtual void setFrameSet(std::string key, std::vector <Texture> textures, FloatingPointType timePerFrame, bool loop) = 0;
		//virtual void setActiveFramesSet(std::string key) = 0;
		//virtual std::string getActiveFramesSet() const = 0;
		virtual FrameId getFrame() const = 0;
		virtual const Texture *getFrameTexture() const = 0;
		//virtual void setFrame(FrameId idx) = 0;
		
		virtual void  setFrameRate(FloatingPointType rate) = 0;
		virtual FloatingPointType getFrameRate() const = 0;
		// 1-> Normal, 2->Twice, 0->frozeen
		virtual void playFrameSet(std::string key) = 0;
		virtual void stopPlayFrameSet() = 0;
	protected:
		// If true a frame have changed
		virtual bool updateCurrentFrame(const Present &present) = 0;
	};
}
