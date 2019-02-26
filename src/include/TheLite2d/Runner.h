#pragma once
#include <memory>
#include "TheLite2d/Drawer.h"
#include "TheLite2d/SDLWrapper.h"
#include "TheLite2d/events/Event.h"

namespace thelite2d {
	// This is a helper for main loop
	class Runner {
	public:
		Runner(SDLWrapper &sdl, std::shared_ptr<Drawer> drawer);
		void run();
	private:
		SDLWrapper &sdl;
		std::shared_ptr<Drawer> drawerRoot;
		void drawOnScreen(SDLWrapper &sdl, Drawer *drawerRoot, const Present & present);
		bool thereAreQuitEvent(std::list<std::unique_ptr<events::Event> > &events);
	};
}