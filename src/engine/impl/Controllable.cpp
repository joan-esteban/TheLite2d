#include "TheLite2d/impl/Controllable.h"


void thelite2d::impl::Controlable::setController(std::shared_ptr<IController> newController)
{
	//Object * p = dynamic_cast<Object*>(this);
	if (controller) controller->abort({}, *owner );
	controller = newController;
	controllerFirstExecution = true;
}

void thelite2d::impl::Controlable::clearController()
{
	if ( (controller) && (!controllerFirstExecution) ) controller->abort({}, *owner);
	controller = nullptr;
}

void thelite2d::impl::Controlable::executeController(const Present &present, Object &owner) {
	if (controller) {
		if (controllerFirstExecution) {
			if (!controller->initialize(present, owner)) {
				//SPDLOG_INFO(getLogger(), "Deleting controller because can't initialize");
				clearController();
				return;
			}
			controllerFirstExecution = false;
		}
		if (!controller->step(present, owner)) {
			//SPDLOG_INFO(getLogger(), "Deleting controller because can't don't want to be executed anymore");
			controllerFirstExecution = true;
			clearController();
		}
	}
}
