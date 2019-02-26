#pragma once
#include <memory>
#include "IController.h"

namespace thelite2d {
	class IControllable {
	public:
		virtual ~IControllable() = default;
		virtual void setController(std::shared_ptr< IController > newController) = 0;
		virtual void clearController() = 0;
	};

	
}

