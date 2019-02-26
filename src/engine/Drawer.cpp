#include <sstream>
#include "TheLite2d/Drawer.h"
#include "TheLite2d/types.h"
#include "TheLite2d/config.h"
#include "TheLite2d/drawcmd/blit.h"
#include "TheLite2d/drawcmd/text.h"
#include "TheLite2d/drawcmd/rectangle.h"
#include "TheLite2d/Mouse.h"
#include "TheLite2d/log.h"

using namespace thelite2d;


Drawer::~Drawer() {
	// I die and left a lot of orphan childs ;(
	std::lock_guard<std::mutex> lock(mutexItems);
	for (auto drawable : items) {
		drawable->setParent(nullptr);
	};
}

Dimensions Drawer::calculateDimensionsPositionablesContained() const {
	auto positionables = getItems<IPositionable>();
	Point2d orig{ 0,0 };
	Point2d dest{ 0,0 };
	for (auto positionable : positionables) {
		auto itemOrig = positionable->getPosition();
		auto itemDest = itemOrig +  positionable->getDimensions();
		if (itemOrig.x < orig.x) orig.x = itemOrig.x;
		if (itemOrig.y < orig.y) orig.y = itemOrig.y;
		
		if (itemDest.x > dest.x) dest.x = itemDest.x;
		if (itemDest.y > dest.y) dest.y = itemDest.y;
	}
	return dest - orig;
}

bool Drawer::getDrawCommands(const Present & present, std::list<std::unique_ptr<IDrawCommand>>& drawCommands) {
	SPDLOG_DEBUG(getLogger(),"getDrawCommands {0} executeController", getObjectName() );
	executeController(present, *this);
	SPDLOG_DEBUG(getLogger(),"getDrawCommands {0} executeController after", getObjectName() );
	bool result = true;
	
	Point2d calculatedPivot = calcultePivotPoint();
	Point2d translate = getPosition() - calculatedPivot;
	{
		//std::lock_guard<std::mutex> lock(mutexItems);
		auto copyItems = getItems<IDrawable>();
		for (auto drawable : copyItems) {
			// TODO: Improve to dont copy list twice
			auto realDrawable = std::dynamic_pointer_cast<IDrawable>(drawable);
            auto childObj = std::dynamic_pointer_cast<Object>(drawable);
			if (realDrawable) {
				std::list<std::unique_ptr<IDrawCommand>> newDrawCommands;
				SPDLOG_DEBUG(getLogger(),"getDrawCommands {0} : child {1} getDrawCommands",
                            getObjectName(), childObj->getObjectName() );
				result &= realDrawable->getDrawCommands(present, newDrawCommands);
				SPDLOG_DEBUG(getLogger(),"getDrawCommands {0} : child getDrawCommands = {1}",
							getObjectName(), newDrawCommands.size() );
				for (auto &drawCommand : newDrawCommands) {
					drawCommand->applyTransformation(getFullData());
					drawCommands.push_back(std::move(drawCommand) );
				}
			};
		}
	}
	removePending();
	addPending();

	if (config::debugMode) {
		std::stringstream ss;
		ss << " DRAWER:" << getObjectName()<<  "pos:" << getPosition() << "  dim:" << getDimensions() << " childs:" << items.size()  ;
		auto pos = getFullData();
		drawCommands.push_back(std::unique_ptr<IDrawCommand>(new drawcmd::Text{ pos,ss.str(), SDL_Color{ 32,64,128,255 } }));
		drawCommands.push_back(std::unique_ptr<IDrawCommand>(new drawcmd::Rectangle{ pos }));

	}
	return result;
}

bool thelite2d::Drawer::onEvent(const events::Event * event) 
{
	events::Event ev{ *event };
	transformEvent(&ev);
	//std::lock_guard<std::mutex> lock(mutexItems);
	auto copyItems = getItems<IEventConsumer>();
	for (auto drawable : copyItems) {
		auto consumer = std::dynamic_pointer_cast<IEventConsumer>(drawable);
		if (consumer.get()) {
			if (consumer->onEvent(&ev)) return true;
		}
	}
	return false;
}

void thelite2d::Drawer::transformEvent(events::Event *event) const {
	event->applyTransformation(getFullData());
}
void thelite2d::Drawer::add(std::shared_ptr<IInsertable> &drawable, ADD_ORDER order) {
	if (drawable.get() == nullptr) return;
	if (mutexItems.try_lock()) {
		_add(drawable, order);
		mutexItems.unlock();
	}
	else {
		pendingToAdd.push_back({ drawable,order });
	}
}

void thelite2d::Drawer::remove(std::shared_ptr<IInsertable> &drawable) {
	if (drawable.get() == nullptr) return;
	drawable->setParent(nullptr);
	if (mutexItems.try_lock()) {
		items.remove(drawable);
		mutexItems.unlock();
	} else  pendingToRemove.push_back(drawable.get());
}

void thelite2d::Drawer::remove(IInsertable *insertable) {
	if (insertable == nullptr) return;
	insertable->setParent(nullptr);
	if (mutexItems.try_lock()) {
		_remove(insertable);
		mutexItems.unlock();
	} else pendingToRemove.push_back(insertable);
}

void thelite2d::Drawer::clear() {
	if (mutexItems.try_lock()) {
		for (auto &item : items) {
			item->setParent(nullptr);
		}
		items.clear();
		mutexItems.unlock();
	}
	else {
		for (const auto &item : items) {

			pendingToRemove.push_back(item.get());
		}
	}
}

void thelite2d::Drawer::_remove(IInsertable *obj) {
	//SPDLOG_INFO(getLogger(), "_remove ({0:x})", obj);
	//items.erase(std::remove_if(items.begin(),items.end(),
	//	[=](const std::shared_ptr<IInsertable> &item) {
	//	return (item.get() == obj); }));
	for (std::list< std::shared_ptr<IInsertable> >::iterator item = items.begin(); item != items.end();) {
		if (item->get() == obj) {
			item = items.erase(item);
		}
		else item++;
	}
}

void thelite2d::Drawer::setParent(IContainer *newParent) {
	parent = newParent;
}

IContainer *thelite2d::Drawer::getParent() const {
	return parent;
}

void thelite2d::Drawer::removePending()
{
	std::lock_guard<std::mutex> lock(mutexItems);
	for (auto toRemove : pendingToRemove) _remove(toRemove);
	pendingToRemove.clear();
}
void thelite2d::Drawer::_add(std::shared_ptr<IInsertable> &drawable, ADD_ORDER order) {
	drawable->setParent(this);
	switch (order) {
	case ADD_ORDER::ON_BOTTOM:
		items.push_back(drawable);
		break;
	case ADD_ORDER::ON_TOP:
		items.push_front(drawable);
		break;
	}
}
void thelite2d::Drawer::addPending()
{
	std::lock_guard<std::mutex> lock(mutexItems);
	for (auto toAdd : pendingToAdd) _add(toAdd.first, toAdd.second);
	pendingToAdd.clear();
}

std::weak_ptr<IMouseReactable>  thelite2d::Drawer::getOnTopMouseReactableAtPosition(events::Event &event, bool eventOnLocalCoordinates) const {
	std::list< std::weak_ptr<IMouseReactable> > result;
	events::Event copyEvent = event;
	if (!eventOnLocalCoordinates) transformEvent(&copyEvent);
	for (auto item : items) {
		auto drawer = std::dynamic_pointer_cast<Drawer>(item);
		auto mouseReactable = std::dynamic_pointer_cast<IMouseReactable>(item);
		if (mouseReactable) {
			if (mouseReactable->isInside(copyEvent.getWhere())) {
				event = copyEvent;
				return mouseReactable;
			};
		}
		if (drawer) {
			auto a = drawer->getOnTopMouseReactableAtPosition(copyEvent, false);
			if (a.lock()) {
				event = copyEvent;
				return a;
			}
		}

	}
	return std::weak_ptr<IMouseReactable>();
}