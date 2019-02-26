#pragma once

#include "director.h"
#include "ISetter.h"


namespace thelite2d {
	namespace actions {
		class IActionDirectable {
		public:
			/// Called when Action start
			virtual bool initialize(Object &objectToControl) = 0;
			/// Called a long Action is alive (after call initialize)
			virtual bool step(SegmentPercent percent, Object &objectToControl) = 0;
			/// Called when Action is finish (always later than called initialize)
			virtual void finish(Object &objectToControl) = 0;

			virtual ~IActionDirectable() = default;
		};

	}
}