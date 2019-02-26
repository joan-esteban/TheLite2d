#include <sstream>
#include <cassert>
#include "TheLite2d/Mouse.h"
#include "TheLite2d/IPositionable.h"
#include "TheLite2d/config.h"
#include "TheLite2d/drawcmd/text.h"
#include "TheLite2d/log.h"

void thelite2d::Mouse::setParent(IContainer *newParent)
{
	if (newParent == parent) return;
	
	if (focused) {
		setNoFocus(nullptr);
		// TODO: Must send setNoFocus to focused object... but I need a event!...
		SPDLOG_ERROR(getLogger(), "NOT IMPLEMENTED: Cant change parent of mouse if a item are focused ");
		assert(false);
	}
	parent = newParent;
}

thelite2d::IContainer * thelite2d::Mouse::getParent() const
{
	return parent;
}

bool thelite2d::Mouse::getDrawCommands(const Present & present, std::list<std::unique_ptr<IDrawCommand>>& drawCommands)
{
	if (config::debugMode && (lastMousePosition != UndefinedPoint2d) ) {
		 
		std::stringstream ss;
		ss << "(" << lastMousePosition.x << "," << lastMousePosition.y << ")";
		auto item = focused;
		Object *obj = dynamic_cast<Object *>(item.get());
		if (obj) {
			ss << " FOCUS=" << obj->getObjectName();
		};
		RectangleWidgetParams params;
		params.position = lastMousePosition;
		drawCommands.push_back(std::unique_ptr<IDrawCommand>(new drawcmd::Text{ ss.str(), SDL_Color{ 32,64,128,255 } }));
		return true;
	}
	return false;
}

bool thelite2d::Mouse::onEvent(const events::Event * event)
{
	lastMousePosition = event->getWhere();
	auto container = getParent();
	Drawer *drawer = dynamic_cast<Drawer *>(container);

	if (!drawer) return false;

	switch (event->getType()) {
	case events::EventType::MOUSE_BUTTON_DOWN:
		return MouseDown(drawer, event);
		break;
	case events::EventType::MOUSE_BUTTON_UP:
		return MouseUp(drawer, event);
		break;
	case events::EventType::MOUSE_MOTION:
		return MouseMotion(drawer, event);
		break;
	}
	return false;
}

bool thelite2d::Mouse::MouseDown(Drawer *drawer, const events::Event *event) {
	events::Event posEvent = *event;
	isDraging = false;
	// Buscar un IPositionable que a mes sigui IMouseReactuble i que estigui actiu
	auto object = drawer->getOnTopMouseReactableAtPosition(posEvent, true);
	auto mr = object.lock();
	setNoFocus(event);
	if (!mr) {
		isClicked = false;
		return false;
	}

	if (!focused) {
		mr->onFocused(mr.get(), posEvent.getWhere() );
		focused = mr;
		isClicked = true;
		mouseDownPosition = event->getWhere();
	}
	return true;
}
bool thelite2d::Mouse::MouseUp(Drawer *drawer, const events::Event *event) {
	isClicked = false;
	
	if (!isValidFocusedItem()) {
		setNoFocus(event);
		return false;
	}
	
	auto shared = focused;
	setNoFocus(event);
	if (shared) {
		Point2d where = shared->convertToLocalCoordinates(event->getWhere(), getParent());
		if (shared->isInside(where)) {
			shared->onClick(shared.get(), event->getWhere());
		}
		
	}
	else return false; // There is no focus, no this event is not mine
	return true;
}
bool thelite2d::Mouse::isValidFocusedItem() {
	if (!focused) return false;
	if (focused.use_count() <= 1) {
		// I'm the last one, and I don't want this resposability so unfocus and release pointer
		SPDLOG_ERROR(getLogger(), "isValidFocusedItem last reference to object");
		return false;
	}
	IInsertable *insertable = dynamic_cast<IInsertable *>(focused.get());
	if ((insertable) && (insertable->getParent() == nullptr)) {
		SPDLOG_ERROR(getLogger(), "isValidFocusedItem object lose parent");
		return false;
	}
	return true;
}
bool thelite2d::Mouse::MouseMotion(Drawer *drawer, const events::Event *event) {
	//SPDLOG_INFO(getLogger(), "Motion : {0},{1}", event->getWhere().x, event->getWhere().y);
	if (!isValidFocusedItem()) {
		setNoFocus(event);
		return false;
	}
	
	if (isClicked)  {
		Point2d where = focused->convertToLocalCoordinates(event->getWhere(), getParent() );
		if ((mouseDownPosition - where).modulus() > DRAG_THRESHOLD) isDraging = true;
		if (isDraging) focused->onDrag(focused.get(),  mouseDownPosition, where);
		return true;
	}
	return false;
	

}


void thelite2d::Mouse::setNoFocus(const events::Event *event) {
	if (focused) {
		Point2d where = thelite2d::UndefinedPoint2d;
		if (event) where = event->getWhere();
		focused->onUnfocused(focused.get(), where);
		if (isDraging) focused->onDragFinish(focused.get(), mouseDownPosition, lastMousePosition);
	}
	isDraging = false;
	focused = nullptr;
}