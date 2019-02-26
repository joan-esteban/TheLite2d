#pragma once
#include <memory>
#include <list>
#include <cassert>

#include "TheLite2d/IController.h"
#include "TheLite2d/types.h"
#include "TheLite2d/Object.h"
#include "TheLite2d/log.h"
#include "TheLite2d/IContainer.h"
#include "TheLite2d/actions/IAction.h"
#include "TheLite2d/actions/director.h"

namespace thelite2d {

	namespace actions {
		namespace Execute {
			using ExecuteFunction = std::function<bool(Object *notifier, Object *object)>;
			using ExecuteAbortFunction = ExecuteFunction;
			const  ExecuteFunction FunctionNop = [](Object *notifier, Object *object) {
				return true;
			};
			const  ExecuteFunction RemoveFromParentFunction = [](Object *notifier, Object *object) {
				IInsertable *insertable = dynamic_cast<IInsertable*>(object);
				if (insertable) {
					IContainer *container = insertable->getParent();
					if (container) {
						Object *containerObject = dynamic_cast<Object*>(container);
						std::string containerName = "???"; 
						if (containerObject) containerName = containerObject->getObjectName();
						SPDLOG_INFO(getLogger(), "removed [{0}] from parent {1} ", object->getObjectName(), containerName);
						container->remove(insertable);
					}
				}
				return false;
			};


			class Seq : public IActionDirectable, public Object {

			public:
				Seq( ExecuteFunction onStart, ExecuteFunction onFinish, ExecuteAbortFunction onAbort) :
					 onStart{ onStart }, onFinish{ onFinish }, onAbort{ onAbort } {};

				virtual ~Seq() {
					if (!finished && (onAbort != nullptr)) onAbort(this, object);
				}

				bool initialize(Object &objectToControl) override {
					initializeCalled = true;
					finished = false;

					object = &objectToControl;
					IInsertable *insertable = dynamic_cast<IInsertable *>(object);
					if ((insertable) && insertable->getParent() == nullptr) return true;

					if (onStart != nullptr) {
						return onStart(this, object);
					}
					return true;
				};

				bool step(SegmentPercent percent, Object &objectToControl) override {
					if (!initializeCalled) {
						SPDLOG_ERROR(getLogger(), "Can't call step without calling initialize");
						std::terminate();
					};
					return true;
				}

				void finish(Object &objectToControl) override {
					if (!initializeCalled) {
						SPDLOG_ERROR(getLogger(), "Can't call step without calling initialize");
						std::terminate();
					};
					finished = true;
					if (onFinish != nullptr) onFinish(this, object);
				}
				

			

			protected:
				ExecuteFunction onStart = nullptr;
				ExecuteFunction onFinish = nullptr;
				ExecuteAbortFunction onAbort = nullptr;

				FloatingPointType initialTime;
				bool finished = false;
				bool initializeCalled = false;

				Object *object = nullptr;
			};

			class Action : public IController, public Object {
				Action(FloatingPointType timeToReach, ExecuteFunction onStart, ExecuteFunction onFinish, ExecuteAbortFunction onAbort) :
					director{ timeToReach }, seq{ onStart, onFinish, onAbort } {};
			

				virtual bool initialize(const Present &present, Object &objectToControl) {
					director.update(present);
					seq.initialize(objectToControl);
				}

				virtual bool step(const Present &present, Object &objectToControl) {
					director.update(present);
					seq.step(director.getPercentage(), objectToControl);
				}

				virtual bool abort(const Present &present, Object &objectToControl) {
					seq.finish(objectToControl);
					return true;
				}

			public:
				DirectorTime director;
				Seq seq;
			};
			
			inline std::shared_ptr<IActionDirectable> seqInstance( ExecuteFunction onStart, ExecuteFunction onFinish = nullptr, ExecuteAbortFunction onAbort = nullptr) {
				return std::shared_ptr<IActionDirectable>(new Seq{ onStart,onFinish,onAbort });
			};


			inline std::shared_ptr<IController> actionInstance(FloatingPointType timeToReach, ExecuteFunction onStart, ExecuteFunction onFinish = nullptr, ExecuteAbortFunction onAbort = nullptr) {
				return std::shared_ptr<IController>(new Action{ timeToReach,onStart,onFinish,onAbort });
			};

		}
	}
}