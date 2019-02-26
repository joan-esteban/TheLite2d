#pragma once
#include <memory>
#include <list>
#include <cassert>

#include "TheLite2d/IController.h"
#include "TheLite2d/types.h"
#include "TheLite2d/IPositionable.h"
#include "TheLite2d/log.h"
#include "interpolater.h"

namespace thelite2d {
	namespace actions {
		namespace Rotate {
			class Setter : public ISetter<FloatingPointType> {
			public:
				virtual FloatingPointType getInitialValue(Object &objectToControl) override {
					IPositionable *sprite = dynamic_cast<IPositionable *>(&objectToControl);
					return sprite->getAngle();
				}
				virtual void setValue(Object &objectToControl, FloatingPointType v) override {
					IPositionable *sprite = dynamic_cast<IPositionable *>(&objectToControl);
					sprite->setAngle(v);
				};
			};

			template < template <typename> class INTERPOLATER >
			using Seq = ActionDirectable <INTERPOLATER, FloatingPointType, Setter>;

			template < template <typename> class INTERPOLATER >
			inline std::shared_ptr<IActionDirectable> seqInstance(INTERPOLATER<FloatingPointType> interpolater) {
				return std::shared_ptr<IActionDirectable>(new Seq<INTERPOLATER>{ interpolater });
			}

			// TODO: Change for that, but I need a shared_ptr builder
			template < template <typename> class INTERPOLATER >
			using Action = ActionBase <INTERPOLATER, FloatingPointType, Setter>;

			template < template <typename> class INTERPOLATER >
			inline std::shared_ptr<IController> actionInstance(INTERPOLATER<FloatingPointType> interpolater, FloatingPointType timeToReach) {
				return std::shared_ptr<IController>(new Action<INTERPOLATER>{ interpolater,timeToReach });
			}
		}
	}
}
