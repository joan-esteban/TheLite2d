#pragma once
#include <memory>
#include <list>
#include <cassert>

#include "TheLite2d/IController.h"
#include "TheLite2d/types.h"
#include "TheLite2d/log.h"
#include "TheLite2d/actions/ActionBase.h"
// https://msdn.microsoft.com/en-us/library/aa733789(v=vs.60).aspx
// 'identifier' : unreferenced formal parameter
#pragma warning( disable: 4100)
namespace thelite2d {
	namespace actions {
		class Sleep : public IController{
		public:
			Sleep(FloatingPointType timeToReach) : timeToReach{ timeToReach }  {};

			bool initialize(const Present &present, Object &objectToControl) override {
				initialTime = present.timeSeconds;
				return true;
			}

			bool step(const Present &present, Object &objectToControl) override {
				return getPercentage(present) < 1.0;
			}

			bool abort(const Present &present, Object &objectToControl) override {
				return true;
			}

			virtual FloatingPointType getPercentage(const Present &present) const  {
				auto elapsedTime = present.timeSeconds - initialTime;
				return  (elapsedTime / timeToReach);
			}


			static std::shared_ptr<IController> createInstance(FloatingPointType timeToReach) {
				return std::shared_ptr<IController>(new Sleep{ timeToReach });
			}

			FloatingPointType initialTime;
			FloatingPointType timeToReach;
		};

	}
}
#pragma warning( default: 4100)