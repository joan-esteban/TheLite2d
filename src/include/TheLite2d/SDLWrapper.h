#pragma once
#include <memory>
#include <list>
#include "IDrawCommand.h"
#include "events/EventBuilder.h"
#include "TtfFont.h"
namespace thelite2d {
    /*! brief Wrapper for SDL in order to be easier to use
     *
     */
	class IDrawCommand;

    class SDLWrapper {
    public:
        SDLWrapper() = default;
        virtual ~SDLWrapper() = default;
		/// if dimensions are set it do a windowed  else if do fullscreen
        bool initialize(Dimensions dimensions = UndefinedDimensions );
        void run(int msPerFrame = 20);
        void finalize();

        SDL_Renderer *getSdlRenderer() const {return sdlRenderer;};
        SDL_Window *getSdlWindow() const {return sdlWindow;};
        const SDL_Rect &getScreenRect() const { return screenRect;};
		Dimensions getDimensions() const { return { FloatingPointType(screenRect.w), FloatingPointType(screenRect.h) }; };
		int addNewEvents(std::list<std::unique_ptr<events::Event> > &events);
	
		void execute(std::list<std::unique_ptr<IDrawCommand>> &drawCommands);
		// For debug propouse
		void writeText(const std::string &msg, const Point2d &xy, const SDL_Color &color) const;

		void setBackgroundColor(SDL_Color color) { backgroundColor = std::move(color); }
		void setDebugFont(std::shared_ptr<TtfFont> font);

    protected:
		

        SDL_Renderer *sdlRenderer = nullptr;
        SDL_Window *sdlWindow = nullptr;
        SDL_Rect screenRect;
		std::shared_ptr<TtfFont> debugFont = nullptr;
		mutable Point2d cursor = { 0,0 };
		Point2d cursorStep = { 0,-20 };
		SDL_Color backgroundColor = { 255,255,255,255 };
    };

} // namespace


