#include "TheLite2d/Button.h"
#include "TheLite2d/events/EventMouseButton.h"
#include "TheLite2d/types.h"
#include "TheLite2d/log.h"
using namespace thelite2d; 

thelite2d::Button::Button(Texture texture) : thelite2d::Sprite{ std::move(texture)  } {

}

bool thelite2d::Button::getDrawCommands(const Present & present, std::list<std::unique_ptr<IDrawCommand>>& drawCommands) {
	
	if ( (bgColor.a > 0) && getEnabled() )  {
		auto pos = getFullData();
		drawCommands.push_back(std::unique_ptr<IDrawCommand>(new drawcmd::Rectangle{ pos, bgColor, true }));
	}
	auto res = Sprite::getDrawCommands(present, drawCommands);
	return res;
}


bool thelite2d::Button::isInside(const Point2d & where)
{
	if (!getEnabled()) return false;
	if (captureEverything) return true;
	Rectangle rect{ getPosition() - calcultePivotPoint() , getScaledDimensions() };
	return rect.isInside(where);
}

Point2d thelite2d::Button::convertToLocalCoordinates(const Point2d & parentCoordinates, IContainer *reachParent)
{
	Point2d local  = parentCoordinates;
	std::list<IPositionable *> parents;
	auto parent = getParent();
	while (parent) {
		if (parent == reachParent) break;
		if (parent) {
			IInsertable *insertable = dynamic_cast<IInsertable*>(parent);
			IPositionable *positionable = dynamic_cast<IPositionable*>(parent);
			if (positionable) parents.push_front(positionable);
			if (insertable) parent = insertable->getParent(); else parent = nullptr;
		}
	}

	for (auto positionable : parents) {
		local = positionable->getFullData().inverseTransform(local);
	}
	return  local;
}

