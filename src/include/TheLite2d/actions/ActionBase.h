#pragma once

#include "director.h"
#include "ISetter.h"
#include "TheLite2d/log.h"
#include "TheLite2d/actions/IAction.h"

namespace thelite2d {
namespace actions {
	constexpr bool isActionEnded(FloatingPointType percent) { return percent >= 0.99999; }
	constexpr bool isActionEnded(SegmentPercent percent) {	return isActionEnded(percent.end); }

	
	
	template < template <typename> class INTERPOLATER,  typename TYPE_DATA, typename SETTER >
	class ActionDirectable : public IActionDirectable {
	public:
		ActionDirectable(INTERPOLATER<TYPE_DATA> interpolater) : interpolater{ interpolater }  {};

		bool initialize(Object &objectToControl) override {
			if (interpolater.isNeedInitialValue()) {
				interpolater.setInitialValue(setter.getInitialValue(objectToControl));
			}
			return true;
		}

		bool step(SegmentPercent percent, Object &objectToControl) override {
			setter.setValue(objectToControl, interpolater.getDelta(percent.end));
			return true;
		}

		void finish(Object &objectToControl) override {
			setter.setValue(objectToControl, interpolater.getDelta(1));
		}


	protected:
		//FloatingPointType initialTime;
		//FloatingPointType timeToReach;
		INTERPOLATER<TYPE_DATA> interpolater;
		SETTER setter;

	};

	template < template <typename> class INTERPOLATER, typename TYPE_DATA, typename SETTER >
	class ActionBase : public IController {
	public:
		ActionBase(INTERPOLATER<TYPE_DATA> interpolater, FloatingPointType timeToReach) : 
			actionDirectable{ interpolater },
			director{ timeToReach } {};

		bool initialize(const Present &present, Object &objectToControl) override {
			director.reset();
			director.update(present);
			return actionDirectable.initialize(objectToControl);
		}

		bool step(const Present &present, Object &objectToControl) override {
			director.update(present);
			auto percent = director.getPercentage();
			if (isActionEnded(percent)) {
				actionDirectable.finish(objectToControl);
				return false;
			};
			return actionDirectable.step(percent, objectToControl);
		}
		virtual bool abort(const Present &present, Object &objectToControl) override {
			actionDirectable.finish(objectToControl);
			return true;
		}

	protected:
		ActionDirectable<INTERPOLATER, TYPE_DATA, SETTER> actionDirectable;
		DirectorTime director;

	};
}
}