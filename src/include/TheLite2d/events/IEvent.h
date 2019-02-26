#pragma once
#include <string>
#include <SDL.h>


namespace thelite2d {
	namespace events {
		enum class EventTypes {
			GENERIC,
			SYSTEM,
			KEYBOARD,
			MOUSE_BUTTON,
			MOUSE_MOTION
		};

		using EventType = EventTypes;
		using EventDataType = int;

		class IEvent {
		public:
			virtual ~IEvent() = default;
			virtual EventType getType() const = 0;
			virtual EventDataType getData() const = 0 ;
			virtual const std::string &getTag() const = 0;
			virtual const SDL_Event &getSdlEvent() const = 0;

			virtual void setTag(std::string &newTag) = 0;
		};
	}
}