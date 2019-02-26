#pragma once
#include "types.h"
namespace thelite2d {
	class Object;

	class IController {
	public:
		virtual ~IController() = default;
		/// returns false is can't initialize
		virtual bool initialize(const Present &present, Object &objectToControl) = 0;
		/// returns true is need more invocations, false all job done
		virtual bool step(const Present &present, Object &objectToControl) = 0;

		virtual bool abort(const Present &present, Object &objectToControl) = 0;
		/// returns [0..1] 
		//virtual FloatingPointType getPercentage(const Present &present) const = 0;
		//virtual void finalize(const Present &present, Object &objectToControl) = 0;
	};
}

