#pragma once
#include <memory>
#include <list>
#include <cassert>

#include "TheLite2d/IController.h"
#include "TheLite2d/types.h"
#include "TheLite2d/log.h"
// https://msdn.microsoft.com/en-us/library/aa733789(v=vs.60).aspx
// 'identifier' : unreferenced formal parameter
#pragma warning( disable: 4100)
namespace thelite2d {
namespace actions {
	
	class SerieGroup : public IController {
	public:
		enum  class Behaviour {
			ONCE,
			LOOP
		};

		SerieGroup() = default;
		SerieGroup(Behaviour  behaviour, std::initializer_list< std::shared_ptr<IController > > l) :behaviour{ behaviour }  {
			actions.insert(actions.end(), l.begin(), l.end());
			originalactions = actions;
		}
		bool initialize(const Present &present, Object &objectToControl) override {
			if (actions.empty()) return false; 
			auto first = actions.front();
			return first->initialize(present, objectToControl);
		}
		bool step(const Present &present, Object &objectToControl) override {
			if (actions.empty()) return false;
			auto first = actions.front();
			bool res =  first->step(present, objectToControl);
			if (!res) {
				actions.pop_front();
				if (actions.empty()) return finished(present, objectToControl);
				 else return initialize(present, objectToControl);
			} 
			return true;

		}

		bool abort(const Present &present, Object &objectToControl) override {
			for (auto action : actions) {
				action->abort(present, objectToControl);
			}
			return true;
		}

	
		bool finished(const Present &present, Object &objectToControl) {
			SPDLOG_INFO(getLogger(), "SerieGroup finished");
			switch (behaviour) {
			case Behaviour::ONCE:
				return false;
			case Behaviour::LOOP:
				return restart(present, objectToControl);
			}
			return false;
		}

		bool restart(const Present &present, Object &objectToControl) {
			actions = originalactions;
			return initialize(present, objectToControl);
		}

		static std::shared_ptr<IController> createInstance(Behaviour  behaviour, std::initializer_list< std::shared_ptr<IController > > l) {
			return std::shared_ptr<IController>(new SerieGroup{ behaviour, l });
		}
		static std::shared_ptr<IController> createInstance(std::initializer_list< std::shared_ptr<IController > > l) {
			return std::shared_ptr<IController>(new SerieGroup{ Behaviour::ONCE, l });
		}

	protected:
		std::list< std::shared_ptr<IController > > originalactions;
		std::list< std::shared_ptr<IController > > actions;
		Behaviour  behaviour;
	};
}
}
#pragma warning( default: 4100)