#pragma once
#include <memory>
#include <list>
#include <cassert>

#include "TheLite2d/IController.h"
#include "TheLite2d/types.h"
#include "TheLite2d/IPositionable.h"
#include "TheLite2d/log.h"
#include "TheLite2d/actions/ActionBase.h"

namespace thelite2d {
	namespace actions {
		namespace Move {
			class Setter : public ISetter<Point2d> {
			public:
				virtual Point2d getInitialValue(Object &objectToControl) override {
					IPositionable *sprite = dynamic_cast<IPositionable *>(&objectToControl);
					return sprite->getPosition();
				}
				virtual void setValue(Object &objectToControl, Point2d v) override {
					IPositionable *sprite = dynamic_cast<IPositionable *>(&objectToControl);
					sprite->setPosition(v);
				};
			};

			template < template <typename> class INTERPOLATER >
			using Seq = ActionDirectable <INTERPOLATER, Point2d, Setter>;

			template < template <typename> class INTERPOLATER >
			inline std::shared_ptr<IActionDirectable> seqInstance(INTERPOLATER<Point2d> interpolater) {
				return std::shared_ptr<IActionDirectable>(new Seq<INTERPOLATER>{ interpolater });
			}

			// TODO: Change for that, but I need a shared_ptr builder
			template < template <typename> class INTERPOLATER >
			using Action = ActionBase <INTERPOLATER, Point2d, Setter>;

			template < template <typename> class INTERPOLATER >
			inline std::shared_ptr<IController> actionInstance(INTERPOLATER<Point2d> interpolater, FloatingPointType timeToReach) {
				return std::shared_ptr<IController>(new Action<INTERPOLATER>{ interpolater,timeToReach });
			};

		}

		namespace MoveX {
			class Setter : public ISetter<FloatingPointType> {
			public:
				virtual FloatingPointType getInitialValue(Object &objectToControl) override {
					IPositionable *sprite = dynamic_cast<IPositionable *>(&objectToControl);
					return sprite->getPosition().x;
				}
				virtual void setValue(Object &objectToControl, FloatingPointType v) override {
					IPositionable *sprite = dynamic_cast<IPositionable *>(&objectToControl);
					sprite->setPosition({ v, UNDEFINED_VALUE } );
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
			};

		}
	

	namespace MoveY {
		class Setter : public ISetter<FloatingPointType> {
		public:
			virtual FloatingPointType getInitialValue(Object &objectToControl) override {
				IPositionable *sprite = dynamic_cast<IPositionable *>(&objectToControl);
				return sprite->getPosition().y;
			}
			virtual void setValue(Object &objectToControl, FloatingPointType v) override {
				IPositionable *sprite = dynamic_cast<IPositionable *>(&objectToControl);
				sprite->setPosition({UNDEFINED_VALUE, v });
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
		};

	}
}
}
