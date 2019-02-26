#include "TheLite2d/SDLEvents.h"
#include "TheLite2d/log.h"
#include <SDL.h>
using namespace thelite2d;

bool SDLEvents::getEvents(std::list<EventGeneric> &events) {
	events.clear();
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		switch (e.type) {
		case SDL_KEYDOWN: 
		case SDL_QUIT: {
			SPDLOG_INFO(getLogger(), "SDL event quit");
			events.push_back(EventGeneric{ EventType::SYSTEM, SDL_QUIT, e });
		} break;
		default:
			SPDLOG_INFO(getLogger(), "Unknown SDL event");
		}
	}
	return !events.empty();
}