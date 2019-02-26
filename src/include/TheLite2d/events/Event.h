
#pragma once
//#include "TheLite2d/events/IEvent.h"
#include <string>
#include <cmath>
#include <SDL.h>
#include "TheLite2d/types.h"
#include "TheLite2d/IPositionable.h"

namespace thelite2d {
	namespace events {
		enum class EventTypes {
			GENERIC,
			SYSTEM,
			KEYBOARD_DOWN,
			MOUSE_BUTTON_UP,
			MOUSE_BUTTON_DOWN,
			MOUSE_MOTION
		};

		using EventType = EventTypes;
		
		
		class Event  {
		public:
			using KEYBOARD_KEY = int;
			Event(Event &&event) = default;
			Event(const Event &event) = default;
			Event(EventType type, KEYBOARD_KEY key, const SDL_Event &sdlEvent) : type{ type }, sdlEvent{ sdlEvent }, key{ key }  {};
			Event(EventType type, const SDL_Event &sdlEvent) : type{ type }, sdlEvent{ sdlEvent } {};
			Event(EventType type, Point2d where, const SDL_Event &sdlEvent) : type{ type }, where{ std::move(where) }, sdlEvent{ sdlEvent } {};
			virtual ~Event() = default;

			EventType getType() const { return type; }
			//EventDataType getData() const { return data; }
			const std::string &getTag() const  { return tag; }
			const SDL_Event &getSdlEvent() const { return sdlEvent; }
			const Point2d &getWhere() const { return where; }
			KEYBOARD_KEY getKey() const { return key;  }
			bool isSetWhere() const { return where != UndefinedPoint2d; }
			void applyTransformation(const RectangleWidgetParams &pos)  {
				if (!isSetWhere()) return;
				where = ((where - ( pos.position - pos.calculatedPivotPosition) )  / pos.scale) ;
			}
			//void setTag(std::string newTag) { tag = std::move(newTag); }
			bool operator==(const Event &ev) const {
				return (type == ev.type) &&
					(tag == ev.tag) &&
					(where == ev.where);
			}

			Event & operator=(const Event &ev) {
				if (this == &ev) return *this;
				type = ev.type;
				tag = ev.tag;
				sdlEvent = ev.sdlEvent;
				where = ev.where;
				return *this;
			}

			bool isRelatedToMouse() const {
				return ( (getType() == EventTypes::MOUSE_BUTTON_UP) ||
					(getType() == EventTypes::MOUSE_BUTTON_DOWN) ||
					(getType() == EventTypes::MOUSE_MOTION) );
			}
		private:
			EventType type;
			std::string tag;
			SDL_Event sdlEvent;
			Point2d where = UndefinedPoint2d;
			KEYBOARD_KEY key = 0;
		};
		
	}
}