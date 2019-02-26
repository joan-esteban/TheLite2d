#pragma once
#include "TheLite2d/IFrameable.h"

namespace thelite2d {
	namespace impl {
		class Frameable : public IFrameable {
		public:
			
			virtual void setFrameSet(std::string key, std::vector <Texture> textures, FloatingPointType timePerFrame = 1.0 / 20.0, bool loop = false) override {
				std::vector <FrameData> frames;
				int i = 0;
				for (auto &texture : textures) {
					FrameData frame{ texture, timePerFrame, ++i };
					frames.push_back(frame);
				}
				if (loop) frames[i - 1].jumpToFrame = 0; else
					frames[i - 1].jumpToFrame = UNSET_FRAME;
				frameSets[key] = frames;
			}

			virtual void playFrameSet(std::string key) override {
				activeFrameSet = key;
				currentFrame = 0;
			}

			virtual void stopPlayFrameSet() override {
				currentFrame = UNSET_FRAME;
			}

			virtual FrameId getFrame() const override {
				return currentFrame;
			}
			virtual const Texture *getFrameTexture() const override {
				auto it = frameSets.find(activeFrameSet);
				if (it == frameSets.end()) return nullptr;
				if (currentFrame == UNSET_FRAME) return nullptr;
				FrameId id = currentFrame % it->second.size();
				const FrameData &data = it->second.at(id);
				return &data.texture;
			}

			virtual void  setFrameRate(FloatingPointType rate) override {
				frameRate = rate;
			}
			virtual FloatingPointType getFrameRate() const override {
				return frameRate;
			}
		protected:
			virtual bool updateCurrentFrame(const Present &present) override {
				
				if (activeFrameSet.empty()) return false;
				if (lastFrameSetAtSecond == UNDEFINED_VALUE) lastFrameSetAtSecond = present.timeSeconds;
				auto it = frameSets.find(activeFrameSet);
				if (it == frameSets.end()) return false; 
				if (currentFrame == UNSET_FRAME) return false;

				FrameId id = currentFrame % it->second.size();
				FrameData &data = it->second.at(id);
				if (lastFrameSetAtSecond + ( data.showDuringInSeconds / frameRate) < present.timeSeconds) {
					// Time to change frame
					currentFrame = data.jumpToFrame;
					lastFrameSetAtSecond = present.timeSeconds;
					// TODO: If I have to move multiples frames?
					//updateCurrentFrame(present);
					return true;
				}
				return false;
			} 
		private:
			std::map<std::string, std::vector <FrameData> > frameSets;
			std::string activeFrameSet = "";
			FrameId currentFrame = UNSET_FRAME;
			FloatingPointType lastFrameSetAtSecond = UNDEFINED_VALUE;
			FloatingPointType frameRate = 1.0;
		};
	}
}