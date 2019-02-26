#pragma once
#include <string>
#include <SDL.h>

//! This define a basic Event with common properties



enum class EventTypes {
	GENERIC,
	SYSTEM,
	KEYBOARD,
	MOUSE
};

using EventType = EventTypes;
using EventDataType = int;

class EventGeneric
{
public:
	EventGeneric(EventType type, EventDataType data, const SDL_Event &sdlEvent) : type{ type }, data{ data }, sdlEvent{ sdlEvent }  {};
	virtual ~EventGeneric() = default;

	// Getters
	EventType getTypeId() const { return type; }
	EventDataType getData() const { return data; }
	std::string getTag() const { return tag; }

	// Setters
	void setTag(std::string &newTag) { tag = newTag; }

protected:
	EventType type;
	EventDataType data;
	std::string tag;
	SDL_Event sdlEvent;
};

