#pragma once
#include <memory>
#include <list>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "TheLite2d/types.h"
#include "TheLite2d/timer.h"
#include "TheLite2d/IController.h"
#include "director.h"
#include "TheLite2d/log.h"
#include "ActionBase.h"
#include "TheLite2d/Object.h"
#include "TheLite2d/IContainer.h"
#include "TheLite2d/config.h"
#include "TheLite2d/drawcmd/text.h"
#include "TheLite2d/log.h"

namespace thelite2d {
	namespace actions {

		/**
		Class Sequencer can set an animation of multiples objects

		It need:
			- DIRECTOR: It decides what is the position of the animation. Usually is time (\ref DirectorTime) but can be a position, %download o whatever
			- ACTIONS: You can add using \ref add each one have a %start, %end, object affected, and an action that must fullfill \ref IActionDirectable
		
		To start you must:
			- Add Sequencer to a parent
			- Parent must be enable

		At finish it deletes all items added (to free them if need)

		*/
		template<class DIRECTOR  = DirectorTime>
		class Sequencer : public Object, public IInsertable, public IDrawable, public IController {
			

		public:
			
			Sequencer(DIRECTOR director) : director{ std::move(director) } {};
			virtual ~Sequencer() = default;

			void setOnAbortDontExecute(bool v) { onAbortDontExecute = v; }


			DIRECTOR &getDirector() { return director; };
			void clear() {
				controllers.clear();
			}
			
			void reset() {
				markAsDirty();
				director.reset();
				for (auto& controller : controllers) {
					controller->reset();
				};
			}
		
			void add(FloatingPointType startingPercent , FloatingPointType finalPercent , std::weak_ptr<Object> object, std::shared_ptr<IActionDirectable> controller) {
				// Not using make_unique to keep under C++11
				auto ptr = std::unique_ptr<BindController>(new BindController{ object ,controller,{ startingPercent ,finalPercent }, false, false, false });
				controllers.push_back(std::move(ptr) );
				markAsDirty();
			}

			void add(FloatingPointType startingPercent , FloatingPointType finalPercent , std::shared_ptr<IActionDirectable> controller) {
				// Not using make_unique to keep under C++11
				controllers.push_back(std::unique_ptr<BindController>(new BindController{ {} ,controller,{ startingPercent ,finalPercent }, false, true,false }));
				markAsDirty();
			}
			

			void remove(std::weak_ptr<Object> object) {
				// TODO
			}

			void finish() {
				controllers.clear();
				auto p = getParent();
				if (p && removeFromParentAtEnd) p->remove(this);
			}

			// Heredado v�a IController
			virtual bool initialize(const Present & present, Object & objectToControl) override {
				director.update(present);
				return true;
			}

			// IController
			virtual bool abort(const Present & present, Object & objectToControl) override {
				if (onAbortDontExecute) return true;
				auto copyControllers = getDataPointer();
				for (auto& controller : copyControllers) {
					BindController *data = controller;
					if (data->initialized) data->controller->step({ 1,0 }, objectToControl);
				}
				return true;
			}
			// IInsertable
			virtual void setParent(IContainer *newParent) override
			{
				parent = newParent;
			}
			// IInsertable
			virtual IContainer * getParent() const override
			{
				return parent;
			}

			// IDrawable
			virtual bool getDrawCommands(const Present & present, std::list<std::unique_ptr<IDrawCommand>>& drawCommands) override {
				step(present, *dynamic_cast<Object *>(getParent()) );

				if (thelite2d::config::debugMode && getObjectName().size() > 0 ) {
					std::stringstream ss;
					ss << " SEQUENCER  [" << getObjectName() << "] (" << std::fixed << std::setprecision(2) << director.getPercentage().end << ")";
					RectangleWidgetParams params;
					params.position = UndefinedPoint2d;
					//drawCommands.push_back(std::make_shared<drawcmd::Text>(params, ss.str(), SDL_Color{ 32,64,128,255 }, true));
					drawCommands.push_back(std::unique_ptr<IDrawCommand>(new drawcmd::Text{ params, ss.str(), SDL_Color{ 64,64,0,255 } }));
					return true;
				}
				return false;
			}
#ifndef __UNIT_TEST__
		private:
#endif
			struct BindController {
				std::weak_ptr<Object> object;
				std::shared_ptr<IActionDirectable> controller;
				SegmentPercent duration; // startingPercent, finalPercent
										 //FloatingPointType startingPercent;
										 //FloatingPointType finalPercent;
				bool initialized;
				bool referenceToSelf;
				bool finalized;

				void reset() {
					initialized = false;
					finalized = false;
				}
			};

			DIRECTOR director;

			std::list< std::unique_ptr<BindController> > controllers;
			IContainer *parent = nullptr;
			bool removeFromParentAtEnd = true;
			mutable bool dirty = true;
			mutable SegmentPercent timeScale = { 0, 1 };
			bool onAbortDontExecute = false;

			void markAsDirty() {
				dirty = true;
			}
			void markAsClean() const {
				dirty = false;
			}
			std::vector<BindController* > getDataPointer() const {
				std::vector<BindController* > result;
				for (auto& controller : controllers) result.push_back(controller.get());
				return result;
			}

			SegmentPercent adjustPercentToItems(SegmentPercent percent, SegmentPercent timeScale) {
				percent = percent * timeScale.getElapsed();
				percent += timeScale.start;
				return percent;
			}
			SegmentPercent getTimeScale() const {
				if (dirty) {
					// Calculate duration
					if (controllers.empty()) {
						markAsClean();
						timeScale = { 0,1 };
						return timeScale;
					}
					const auto &minBind = std::min_element(controllers.begin(), controllers.end(),
						[](const std::unique_ptr<BindController> &a, const std::unique_ptr<BindController> &b) {
						return a->duration.start < b->duration.start; }
					);

					const auto &maxBind = std::max_element(controllers.begin(), controllers.end(),
						[](const std::unique_ptr<BindController> &a, const std::unique_ptr<BindController> &b) {
						return a->duration.end < b->duration.end; }
					);
					auto a = (*minBind)->duration.start;
					timeScale = { (*minBind)->duration.start, (*maxBind)->duration.end };
					markAsClean();
					return timeScale;
				}
				return timeScale;
			}

			SegmentPercent calculateChildPercent(const SegmentPercent &itemDuration, const SegmentPercent &directorSegment) {

				SegmentPercent result{
					(directorSegment.start - itemDuration.start) / itemDuration.getElapsed(),
					(directorSegment.end - itemDuration.start) / itemDuration.getElapsed()
				};

				return result;
			}


			void stepChild(SegmentPercent &percent, BindController* data, Object *targetObject) {
				auto percentChild = calculateChildPercent(data->duration, percent);

				if (percent.isInside(data->duration.start) && !data->initialized && !data->finalized && targetObject) {
					data->controller->initialize(*targetObject);
					data->initialized = true;
				}

				if (percent.isInside(data->duration.end) && data->initialized && !data->finalized && targetObject) {
					data->controller->finish(*targetObject);
					data->initialized = false;
					data->finalized = true;

				}

				if (data->initialized  && targetObject) {
					data->controller->step(percentChild, *targetObject);
				}
			};

			virtual bool step(const Present & present, Object & objectToControl) override {
				director.update(present);
				SegmentPercent directorPercent = director.getPercentage();
				if (thelite2d::actions::isActionEnded(directorPercent)) {
					// IF it last execution must reach 1.0 percent so it's forced
					directorPercent.end = 1.0;
				};
				auto percent = adjustPercentToItems(directorPercent, getTimeScale());
				
				// I need a copy because can be deleted this class by a Notify callback
				auto copyControllers = getDataPointer();
				for (auto& controller : copyControllers) {
					Object *object = nullptr;
					BindController *data = controller;
					if (data->referenceToSelf) object = &objectToControl;
					else {
						auto sharedObject = data->object.lock();
						object = sharedObject.get();
					}
					stepChild(percent, data, object);
				}

				if (thelite2d::actions::isActionEnded(directorPercent)) {
					finish();
					return false;
				};

				return true;
			}
		};
	}
}


