#pragma once
#include <memory>
#include <list>
#include <cassert>

#include "TheLite2d/IController.h"
#include "TheLite2d/types.h"
//#include "TheLite2d/ISprite.h"
#include "TheLite2d/IPositionable.h"
#include "TheLite2d/log.h"
#include "ActionBase.h"

namespace thelite2d {
	namespace actions {
		namespace Copy {
			class Setter : public ISetter<RectangleWidgetParams> {
			public:
				virtual RectangleWidgetParams getInitialValue(Object &objectToControl) override {
					IPositionable *sprite = dynamic_cast<IPositionable *>(&objectToControl);
					return sprite->getFullData();
				}
				virtual void setValue(Object &objectToControl, RectangleWidgetParams v) override {
					IPositionable *sprite = dynamic_cast<IPositionable *>(&objectToControl);
					sprite->setFullData(v);
				};
			};

			template < template <typename> class INTERPOLATER >
			using Seq = ActionDirectable <INTERPOLATER, FloatingPointType, Setter>;

			template < template <typename> class INTERPOLATER >
			inline std::shared_ptr<IActionDirectable> seqInstance(INTERPOLATER<RectangleWidgetParams> interpolater) {
				return std::shared_ptr<IActionDirectable>(new Seq<INTERPOLATER>{ interpolater });
			}

			// TODO: Change for that, but I need a shared_ptr builder
			template < template <typename> class INTERPOLATER >
			using Action = ActionBase <INTERPOLATER, RectangleWidgetParams, Setter>;

			template < template <typename> class INTERPOLATER >
			inline std::shared_ptr<IController> actionInstance(INTERPOLATER<RectangleWidgetParams> interpolater, FloatingPointType timeToReach) {
				return std::shared_ptr<IController>(new Action<INTERPOLATER>{ interpolater,timeToReach });
			}
			
		}
	}
}