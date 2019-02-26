#pragma once
#include <memory>
#include "Texture.h"
#include "events/Event.h"

namespace thelite2d {
	class IEventConsumer {
	public:
		virtual ~IEventConsumer() = default;
		// return true is absorved, false will go on propagation
		virtual bool onEvent(const events::Event *event) = 0;

	};
}