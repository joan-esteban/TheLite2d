
#pragma once
#include <memory>

#include "TheLite2d/events/Event.h"
#include "TheLite2d/types.h"
namespace thelite2d {
namespace events {
	/// This function get a raw event and converted 
	std::unique_ptr<Event> convert(SDL_Event &rawEvent);
}
}