#pragma once
#include <list>
#include "EventGeneric.h"

namespace thelite2d {
	class SDLEvents {
	public:
		/// Returns true if there are any event
		bool getEvents(std::list<EventGeneric> &events);
	};
}