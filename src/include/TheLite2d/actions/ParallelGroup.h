#pragma once
#include <memory>
#include <list>
#include <cassert>

#include "TheLite2d/IController.h"
#include "TheLite2d/types.h"
// https://msdn.microsoft.com/en-us/library/aa733789(v=vs.60).aspx
// 'identifier' : unreferenced formal parameter
#pragma warning( disable: 4100)
namespace thelite2d {
	namespace actions {
		class ParallelGroup : public IController {
		public:
			ParallelGroup() = default;
			ParallelGroup(std::initializer_list< std::shared_ptr<IController > > l) {
				actions.insert(actions.end(), l.begin(), l.end());
			}
			bool initialize(const Present &present, Object &objectToControl) override {
				if (actions.empty()) return false;
				bool result = true;
				for (auto &ctrl : actions) {
					result = result & ctrl->initialize(present, objectToControl);
				}
				return result;
			}
			bool step(const Present &present, Object &objectToControl) override {
				if (actions.empty()) return false;
				bool result = false;
				for (auto &ctrl : actions) {
					result = result | ctrl->step(present, objectToControl);
				}
				return result;

			}
			bool abort(const Present &present, Object &objectToControl) override {
				for (auto &ctrl : actions) {
					ctrl->abort(present, objectToControl);
				}
				return true;
			}
		


			static std::shared_ptr<IController> createInstance(std::initializer_list< std::shared_ptr<IController > > l) {
				return std::shared_ptr<IController>(new ParallelGroup{ l });
			}

			std::list< std::shared_ptr<IController > > actions;
		};
	}
}
#pragma warning( default: 4100)