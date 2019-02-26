#pragma once
#include <memory>
#include <list>
#include <cassert>

#include "TheLite2d/IController.h"
#include "TheLite2d/types.h"
//#include "TheLite2d/ISprite.h"
#include "TheLite2d/log.h"

namespace thelite2d {
namespace actions {
		class Blink : public IController {
		public:
			Blink(FloatingPointType frequency) : frequency{ frequency } {};
			bool initialize(const Present &present, Object &objectToControl) override {
				nextTime = present.timeSeconds;
				IPositionable *sprite = dynamic_cast<IPositionable *>(&objectToControl);
				assert(sprite != nullptr);
				
				return true;
			};
			bool step(const Present &present, Object &objectToControl) override {
				IPositionable *sprite = dynamic_cast<IPositionable *>(&objectToControl);
				//SPDLOG_INFO(getLogger(), "next {0} current{1}", nextTime, present.timeSeconds);
				if (present.timeSeconds > nextTime) {
					sprite->setVisible(!sprite->getVisible());
					nextTime += (1 / frequency );
					//SPDLOG_INFO(getLogger(), "TOOGLE next {0}", nextTime);
				}
				return true;
			}

			bool abort(const Present &present, Object &objectToControl) override {
				return true;
			}

			virtual FloatingPointType getPercentage(const Present &present) const {
				return 0;
			}

			static std::shared_ptr<IController> createInstance(FloatingPointType frequency) {
				return std::shared_ptr<IController>(new Blink{ frequency });
			}

		protected:
			FloatingPointType frequency;
			FloatingPointType nextTime = -1;
		};
}
}
