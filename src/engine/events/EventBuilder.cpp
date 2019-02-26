#include "TheLite2d/events/EventBuilder.h"
#include "TheLite2d/events/EventMouseButton.h"
#include "TheLite2d/log.h"
using namespace thelite2d::events;

std::unique_ptr<Event> thelite2d::events::convert(SDL_Event &rawEvent) {
	switch (rawEvent.type) {
	case SDL_MOUSEBUTTONDOWN:
		//SPDLOG_INFO(getLogger(), " event type SDL_MOUSEBUTTONDOWN {0},{1}", rawEvent.button.x, rawEvent.button.y);
		return std::unique_ptr<Event>(new Event{ EventTypes::MOUSE_BUTTON_DOWN, Point2d{ static_cast<FloatingPointType>(rawEvent.button.x), static_cast<FloatingPointType>(rawEvent.button.y) }, rawEvent });
	case SDL_MOUSEBUTTONUP:
		//SPDLOG_INFO(getLogger(), " event type SDL_MOUSEBUTTONUP {0},{1}", rawEvent.button.x, rawEvent.button.y);
		return std::unique_ptr<Event>(new Event{ EventTypes::MOUSE_BUTTON_UP, Point2d{ static_cast<FloatingPointType>(rawEvent.button.x), static_cast<FloatingPointType>(rawEvent.button.y) }, rawEvent });
	case SDL_MOUSEMOTION:
		//SPDLOG_INFO(getLogger(), " event type SDL_MOUSEMOTION {0},{1}", rawEvent.button.x, rawEvent.button.y);
		return std::unique_ptr<Event>(new Event{ EventTypes::MOUSE_MOTION, Point2d{ static_cast<FloatingPointType>(rawEvent.motion.x), static_cast<FloatingPointType>(rawEvent.motion.y) }, rawEvent });
	case SDL_KEYDOWN: {
		auto key = rawEvent.key.keysym.sym;
		if (key == SDLK_ESCAPE) return std::unique_ptr<Event>(new Event{ EventTypes::SYSTEM, rawEvent });
		return std::unique_ptr<Event>(new Event{ EventTypes::KEYBOARD_DOWN, key, rawEvent });
	}
	case SDL_QUIT: 
		return std::unique_ptr<Event>(new Event{ EventTypes::SYSTEM, rawEvent });
	default:
		return std::unique_ptr<Event>(new Event{ EventTypes::GENERIC,  rawEvent });
	}
}