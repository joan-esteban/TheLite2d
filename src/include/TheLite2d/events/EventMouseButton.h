
#pragma once
#include "TheLite2d/events/Event.h"
#include "TheLite2d/types.h"
/*
namespace jer {
namespace events {
	enum class  ButtonAction {
		DOWN, //!< Event is a mouse down
		UP,   //!< Event is a mouse up
	};
	class EventMouseButton : public Event {
	public:
		EventMouseButton(ButtonAction action, Point2d where, const SDL_Event &sdlEvent) : Event{ EventTypes::MOUSE_BUTTON, {}, sdlEvent },
				action{ action }, where{ std::move(where)} {};
		virtual ~EventMouseButton() = default;
		
		ButtonAction getButtonAction() const { return action; };
		const Point2d &getWhere() const { return where; };

	private:
		ButtonAction action;
		Point2d where;
	};
}
}
*/