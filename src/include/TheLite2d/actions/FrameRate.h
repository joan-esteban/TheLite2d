#pragma once
#pragma once
#include <memory>
#include <list>
#include <cassert>

#include "TheLite2d/IController.h"
#include "TheLite2d/types.h"
//#include "TheLite2d/ISprite.h"
#include "TheLite2d/IPositionable.h"
#include "TheLite2d/log.h"
#include "TheLite2d/Sprite.h"
#include "ActionBase.h"

namespace thelite2d {
	namespace actions {
		namespace FrameRate {
			class Setter : public ISetter<FloatingPointType> {
			public:
				virtual FloatingPointType getInitialValue(Object &objectToControl) override {
					Sprite *sprite = dynamic_cast<Sprite *>(&objectToControl);
					return sprite->getFrameRate();
				}
				virtual void setValue(Object &objectToControl, FloatingPointType v) override {
					Sprite *sprite = dynamic_cast<Sprite *>(&objectToControl);
					sprite->setFrameRate(v);
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
