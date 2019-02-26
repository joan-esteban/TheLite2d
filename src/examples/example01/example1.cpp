#include <string>
#include <sstream>

#include "TheLite2d/TheLite2d.h"
/// This is example 01
/// It shows a Hello World!!

using namespace thelite2d;



int main(int argc, char* args[])
{
	thelite2d::config::debugMode = true;
	// Initialize SPDLOG
	initLogger();
	SPDLOG_INFO(getLogger(), "Hello World!");
	// Initialize Gfx
	SDLWrapper sdl;
	if (!sdl.initialize(Dimensions{ 800.0, 600.0 } )) {
		SPDLOG_ERROR(getLogger(), "error initializing sdl");
		return 1;
	}
	// Set search path for assert
	utils::Files files;
	thelite2d::utils::FindAssets findAssets({ "../../src/examples/assets/",
		files.basename(args[0]) + "../../src/examples/assets/"  /// Visual Studio runs executable at root folder, so to find assets need a trick
	});
	// Create a font
	auto font = TtfFont::createInstance(findAssets.get("clacon.ttf"), 20);
	sdl.setDebugFont(font);
	sdl.setBackgroundColor({ 128,64,64,255 });

	// Create main drawer
	std::shared_ptr<Drawer> drawerRoot = Drawer::createInstance("ROOT");
	drawerRoot->setDimensions( sdl.getDimensions() );
	drawerRoot->setPivot(PivotMode::ON_TOP_LEFT);
	drawerRoot->setPosition({ 0,0 });
	
	// Add something to main drawer
	auto txtHello = Sprite::createInstance(TextureTtf::createInstance(sdl, font, "Hello world!!!", SDL_Color{ 255,255,255,0 }));
	txtHello->setPosition(drawerRoot->getDimensions() / 2);
	txtHello->setPivot(PivotMode::ON_MIDDLE);
	drawerRoot->add(txtHello);
	// Add some action
	auto seq = std::make_shared<actions::Sequencer<> >(actions::DirectorTime{ 1.0 });
	seq->add(0.0, 0.8, txtHello, actions::Zoom::seqInstance<UniformLineMotion>({ { 0.0, 0.0 } ,{ 4.0, 4.0 } }));
	seq->add(0.8, 1, txtHello, actions::Zoom::seqInstance<UARMotion>({ { 4.0, 4.0 } ,{ 2.0, 2.0 },{ 0,0 } }));
	seq->add(0.5, 1, txtHello, actions::Execute::seqInstance([&](Object *notifier, Object *object) {
		SPDLOG_INFO(getLogger(), "Hello World!");
		seq->reset(); 
		return true;
	}));
	drawerRoot->add(seq);

	// And run it!
	Runner runner(sdl, drawerRoot);
	runner.run();
	return 0;
}