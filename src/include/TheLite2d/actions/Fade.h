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
		namespace Fade {
			class Setter : public ISetter<FloatingPointType> {
			public:
				virtual FloatingPointType getInitialValue(Object &objectToControl) override {
					IPositionable *sprite = dynamic_cast<IPositionable *>(&objectToControl);
					return sprite->getAlpha();
				}
				virtual void setValue(Object &objectToControl, FloatingPointType v) override {
					IPositionable *sprite = dynamic_cast<IPositionable *>(&objectToControl);
					sprite->setAlpha(v);
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

			/*

			template < template <typename> class INTERPOLATER >
			class Fade : public ActionBase<INTERPOLATER, FloatingPointType, Setter> {
			public:
				Fade(INTERPOLATER<FloatingPointType> interpolater, FloatingPointType timeToReach) : ActionBase<INTERPOLATER, FloatingPointType, Setter>{ interpolater,timeToReach } {};


				static std::shared_ptr<IController> createInstance(INTERPOLATER<FloatingPointType> interpolater, FloatingPointType timeToReach) {
					return std::shared_ptr<IController>(new Fade<INTERPOLATER>{ interpolater, timeToReach });
				}
			};
			*/
		}
	}
}
