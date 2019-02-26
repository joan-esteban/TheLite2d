#pragma once
#include <memory>
#include <list>
#include <cassert>

#include "TheLite2d/IController.h"
#include "TheLite2d/types.h"
#include "TheLite2d/IPositionable.h"
#include "TheLite2d/log.h"
#include "TheLite2d/actions/ActionBase.h"
#include "ISetter.h"

namespace thelite2d {
	namespace actions {
		namespace Zoom {
			class Setter : public ISetter<ScaleFactor> {
			public:
				virtual ScaleFactor getInitialValue(Object &objectToControl) override {
					IPositionable *sprite = dynamic_cast<IPositionable *>(&objectToControl);
					return sprite->getScale();
				}
				virtual void setValue(Object &objectToControl, ScaleFactor v) override {
					IPositionable *sprite = dynamic_cast<IPositionable *>(&objectToControl);
					sprite->setScale(v);
				};
			};


			template < template <typename> class INTERPOLATER >
			using Seq = ActionDirectable <INTERPOLATER, ScaleFactor, Setter>;

			template < template <typename> class INTERPOLATER >
			inline std::shared_ptr<IActionDirectable> seqInstance(INTERPOLATER<ScaleFactor> interpolater) {
				return std::shared_ptr<IActionDirectable>(new Seq<INTERPOLATER>{ interpolater });
			}

			// TODO: Change for that, but I need a shared_ptr builder
			template < template <typename> class INTERPOLATER >
			using Action = ActionBase <INTERPOLATER, ScaleFactor, Setter>;

			template < template <typename> class INTERPOLATER >
			inline std::shared_ptr<IController> actionInstance(INTERPOLATER<ScaleFactor> interpolater, FloatingPointType timeToReach) {
				return std::shared_ptr<IController>(new Action<INTERPOLATER>{ interpolater,timeToReach });
			};
			/*
			template < template <typename> class INTERPOLATER >
			class Zoom : public ZoomSetter,  public ActionBase<INTERPOLATER, ScaleFactor, ZoomSetter> {
			public:
				Zoom(INTERPOLATER<ScaleFactor> interpolater, FloatingPointType timeToReach) : ActionBase<INTERPOLATER, ScaleFactor, ZoomSetter>{ interpolater,timeToReach } {};

				static std::shared_ptr<IController> createInstance(INTERPOLATER<ScaleFactor> interpolater, FloatingPointType timeToReach) {
					return std::shared_ptr<IController>(new Zoom<INTERPOLATER>{ interpolater, timeToReach });
				}
			};
			*/
		}
	}
}
