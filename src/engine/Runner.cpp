#include "TheLite2d/Runner.h"
#include "TheLite2d/Timer.h"

thelite2d::Runner::Runner(SDLWrapper &sdl, std::shared_ptr<Drawer> drawer) :
	sdl{ sdl }, drawerRoot{ std::move(drawer) }{
}
void thelite2d::Runner::run() {
	std::list<std::unique_ptr<events::Event> > events;
	Timer timer;
	while (!thereAreQuitEvent(events)) {
		sdl.addNewEvents(events);
		for (const auto &event : events) drawerRoot->onEvent(event.get());
		drawOnScreen(sdl, drawerRoot.get(), timer.current());
		timer.next();
	}
}
void thelite2d::Runner::drawOnScreen(SDLWrapper &sdl, Drawer *drawerRoot, const Present & present) {
	if (!drawerRoot) return;
	std::list<std::unique_ptr<IDrawCommand>> drawCommands;
	drawerRoot->getDrawCommands(present, drawCommands);
	sdl.execute(drawCommands);
}
bool thelite2d::Runner::thereAreQuitEvent(std::list<std::unique_ptr<events::Event> > &events) {
	for (const auto &event : events) {
		if (event->getType() == events::EventType::SYSTEM) return true;
	}
	return false;
}
