#include "TheLite2d/KeyboardDebug.h"
#include "TheLite2d/drawcmd/text.h"
#include "TheLite2d/config.h"
#include "TheLite2d/log.h"
using namespace thelite2d;

void thelite2d::KeyboardDebug::setParent(IContainer *newParent)
{
	parent = newParent;
}

IContainer * thelite2d::KeyboardDebug::getParent() const
{
	return parent;
}

bool thelite2d::KeyboardDebug::getDrawCommands(const Present & present, std::list<std::unique_ptr<IDrawCommand>>& drawCommands)
{
#ifdef __DEBUG__
	RectangleWidgetParams params;
	params.position =  UndefinedPoint2d;;
	drawCommands.push_back(std::unique_ptr<IDrawCommand>(new drawcmd::Text{ "KEYBOARD DEBUG", SDL_Color{ 32,64,128,255 } }));
#endif
	return true;
}

bool thelite2d::KeyboardDebug::onEvent(const events::Event * event)
{
	auto pos = dynamic_cast<IPositionable *>(getParent());
	if (!pos) return false;
	const FloatingPointType deltaPosition = 50.0;
	const Dimensions deltaScale = { 1.1, 1.1 };
	if (event->getType() == events::EventTypes::KEYBOARD_DOWN) {
		switch (event->getKey()) {
		case SDLK_KP_MINUS:
		case SDLK_MINUS: {
			auto newScale = pos->getScale() / deltaScale;
			pos->setScale(newScale);
			return true;
		}
		case SDLK_KP_PLUS:
		case SDLK_PLUS: {
			pos->setScale(pos->getScale() * deltaScale);
			return true;
		}
		case SDLK_UP: {
			pos->setPosition(pos->getPosition() - Point2d{ 0, deltaPosition });
			return true;
		}
		case SDLK_DOWN: {
			pos->setPosition(pos->getPosition() + Point2d{ 0, deltaPosition });
			return true;
		}

		case SDLK_LEFT: {
			pos->setPosition(pos->getPosition() - Point2d{ deltaPosition, 0});
			return true;
		}
		case SDLK_RIGHT: {
			pos->setPosition(pos->getPosition() + Point2d{ deltaPosition, 0 });
			return true;
		}
		case SDLK_1:
		case SDLK_KP_ENTER: {
			config::debugMode = !config::debugMode;
			SPDLOG_ERROR(getLogger(), "Changing debugMode {0}", config::debugMode);
			return true;
		}
		}
	}
	return false;

}
