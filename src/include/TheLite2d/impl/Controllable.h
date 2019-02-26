#pragma once
#include "TheLite2d/IControllable.h"

/// This is a helper class to don't repeat this code
namespace thelite2d {
	namespace impl {
		class Controlable : virtual public IControllable {
		public:
			Controlable(Object *owner) :owner{ owner } {};
			virtual ~Controlable() = default;
			// Heredado v�a IControllable
			virtual void setController(std::shared_ptr<IController> newController) override;
			virtual void clearController() override;

		protected:
			virtual void executeController(const Present &present, Object &owner);

			bool controllerFirstExecution = true;
			std::shared_ptr<IController > controller = nullptr;
			Object *owner = nullptr;
		};
	}
}